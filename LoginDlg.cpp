// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CString SessionDlgUserID, SessionDlgPassword;
void SessionDlgSetStateMessage(LPVOID lParam, CString Message, int CurProgress, int GoalProgress) {
	CLoginDlg* pLoginDlg = (CLoginDlg*)lParam;
	if(pLoginDlg != NULL) {
		CStatic *StaticMessage		= (CStatic*)pLoginDlg->GetDlgItem(IDC_STATICMESSAGE);
		CProgressCtrl *ProgressBar	= (CProgressCtrl*)pLoginDlg->GetDlgItem(IDC_LOGINPROGRESS);

		if(StaticMessage != NULL &&
			ProgressBar != NULL) {

			ProgressBar->SetPos(CurProgress);
			for(int i=CurProgress; i<GoalProgress; i++) {
				ProgressBar->StepIt();
				Sleep(10);
			}
			StaticMessage->SetWindowText(Message);
		}
	}
}

UINT SessionDlgCheckSessionThread(LPVOID lParam) {

	CHttpWrapper httpWrapper;
	bool bSuccess = false;

	SessionDlgSetStateMessage(lParam, MSG_PREPAREWORK, 0, 0);
	SessionDlgSetStateMessage(lParam, MSG_PREPAREHTTPCONNECT, 0, 30);

	CLoginDlg* pLoginDlg = (CLoginDlg*)lParam;
	if(!httpWrapper.CheckSession()) {
		pLoginDlg->SetLoginState(true);
	}else {
		pLoginDlg->SetLoginState(false);
	}

	SessionDlgSetStateMessage(lParam, MSG_COMPLETEEND, 100, 100);
	return 0;
}

UINT SessionDlgLogoutThread(LPVOID lParam) {

	CHttpWrapper httpWrapper;
	bool bSuccess = false;

	SessionDlgSetStateMessage(lParam, MSG_PREPAREWORK, 0, 0);
	SessionDlgSetStateMessage(lParam, MSG_PREPAREHTTPCONNECT, 0, 30);

	if(!httpWrapper.SetLogout(SessionDlgUserID)) {
		SessionDlgSetStateMessage(lParam, MSG_COMPLETEFILUREWORK + httpWrapper.GetLastError(), 100, 100);
		AfxMessageBox(LOGOUT_INCORRECT_ALERT);

	}else {
		SessionDlgSetStateMessage(lParam, MSG_COMPLETESUCCESSWORK, 100, 100);

		CConfiguration configuration;
		configuration.SetLoginUserID("");
		AfxMessageBox(LOGOUT_CORRECT_ALERT);

		CLoginDlg* pLoginDlg = (CLoginDlg*)lParam;
		pLoginDlg->SetLoginState(true);
	}
	return 0;
}

UINT SessionDlgLoginThread(LPVOID lParam) {

	CHttpWrapper httpWrapper;
	bool bSuccess = false;

	SessionDlgSetStateMessage(lParam, MSG_PREPAREWORK, 0, 0);
	SessionDlgSetStateMessage(lParam, MSG_PREPAREHTTPCONNECT, 0, 30);

	if(!httpWrapper.SetLogin(SessionDlgUserID, SessionDlgPassword)) {
		SessionDlgSetStateMessage(lParam, MSG_COMPLETEFILUREWORK + httpWrapper.GetLastError(), 100, 100);
		AfxMessageBox(LOGIN_INCORRECT_ALERT);
	}else {
		SessionDlgSetStateMessage(lParam, MSG_COMPLETESUCCESSWORK, 30, 100);

		CConfiguration configuration;
		configuration.SetLoginUserID(SessionDlgUserID);
		AfxMessageBox(LOGIN_CORRECT_ALERT);

		CLoginDlg* pLoginDlg = (CLoginDlg*)lParam;
		SendMessage(pLoginDlg->m_hWnd, WM_CLOSE, NULL, NULL);
	}
	return 0;
}

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strUserID = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT

	m_bLoginState	= false;
	httpWrapper		= new CHttpWrapper();
	configuration	= new CConfiguration();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONSESSIONDLG);
}

CLoginDlg::~CLoginDlg() {
	delete httpWrapper;
	delete configuration;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_TXTUSERID, m_strUserID);
	DDX_Text(pDX, IDC_TXTPASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LOGINPROGRESS, m_ctrlLoginProgress);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_BTNLOGIN, OnBtnLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

void CLoginDlg::OnBtnLogin() 
{
	if(UpdateData(TRUE)) {
		m_ctrlLoginProgress.SetPos(0);

		if(m_bLoginState) {
			if(m_strUserID.Trim() == "") {
				AfxMessageBox(LOING_EMPTYID_ALERT);
				CEdit* UserID = (CEdit*)GetDlgItem(IDC_TXTUSERID);
				UserID->SetFocus();
				return;
			}

			if(m_strPassword.Trim() == "") {
				AfxMessageBox(LOING_EMPTYPASSWORD_ALERT);
				CEdit* Password = (CEdit*)GetDlgItem(IDC_TXTPASSWORD);
				Password->SetFocus();
				return;
			}

			SessionDlgUserID	= m_strUserID;
			SessionDlgPassword	= m_strPassword;

			AfxBeginThread(SessionDlgLoginThread, (LPVOID)this);

		}else {
			SessionDlgUserID = configuration->GetLoginUserID();
			AfxBeginThread(SessionDlgLogoutThread, (LPVOID)this);
		}
	}
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlLoginProgress.SetRange(0, 100);
	m_ctrlLoginProgress.SetStep(1);
	m_ctrlLoginProgress.SetPos(0);

	AfxBeginThread(SessionDlgCheckSessionThread, (LPVOID)this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::SetLoginState(bool bLogin) {
	CEdit* txtUserID	= (CEdit*)GetDlgItem(IDC_TXTUSERID);
	CEdit* txtPassword	= (CEdit*)GetDlgItem(IDC_TXTPASSWORD);
	CButton* btnLogin	= (CButton*)GetDlgItem(IDC_BTNLOGIN);

	m_bLoginState = bLogin;

	if(bLogin) {
		txtUserID->EnableWindow(TRUE);
		txtPassword->EnableWindow(TRUE);
		btnLogin->SetWindowText(LOGIN_STATE_LOGIN);
	}else {
		txtUserID->EnableWindow(FALSE);
		txtPassword->EnableWindow(FALSE);
		btnLogin->SetWindowText(LOGIN_STATE_LOGOUT);
	}
}