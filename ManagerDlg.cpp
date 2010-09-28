// ManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "ManagerDlg.h"
#include "HtmlParser.h"
#include "HttpWrapper.h"
#include "ConfigurationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManagerDlg dialog

#define MANAGEDLG_REDUCE_SIZE	180
#define MANAGEDLG_COLLAPSE_SIZE	450

CString ManagerDlgUserID, ManagerDlgSubject, ManagerDlgCurrentLocation, ManagerDlgOrgContents;
bool bManagerDlgExistStateThread = false;

void ManagerDlgThreadEnd(LPVOID lParam, bool bSuccess=true) {
	CManagerDlg* pManagerDlg = (CManagerDlg*)lParam;
	CAnimateCtrl *AnimateCtrl = (CAnimateCtrl*)pManagerDlg->GetDlgItem(IDC_ANIMATE);
	CButton *BtnSavePost = (CButton*)pManagerDlg->GetDlgItem(IDC_BTNSAVEPOST);
	CButton *BtnCancel = (CButton*)pManagerDlg->GetDlgItem(IDCANCEL);
	CButton *BtnAutoClose = (CButton*)pManagerDlg->GetDlgItem(IDC_CHKAUTOCLOSE);

	AnimateCtrl->Stop();
	AnimateCtrl->Seek(0);
	BtnSavePost->EnableWindow(TRUE);
	BtnCancel->EnableWindow(TRUE);

	if(bSuccess && BtnAutoClose->GetCheck() == TRUE)
		SendMessage(pManagerDlg->m_hWnd, WM_CLOSE, NULL, NULL);
}

void ManagerDlgSetStateMessage(LPVOID lParam, CString Message, int CurProgress, int GoalProgress) {
	CManagerDlg* pManagerDlg = (CManagerDlg*)lParam;
	if(pManagerDlg != NULL) {
		CStatic *StaticMessage		= (CStatic*)pManagerDlg->GetDlgItem(IDC_STATICMESSAGE);
		CStatic *StaticPersent		= (CStatic*)pManagerDlg->GetDlgItem(IDC_PERCENT);
		CProgressCtrl *ProgressBar	= (CProgressCtrl*)pManagerDlg->GetDlgItem(IDC_PROGRESS);

		if(StaticMessage != NULL &&
			ProgressBar != NULL) {

			CString strPersent;
			ProgressBar->SetPos(CurProgress);
			for(int i=CurProgress; i<GoalProgress; i++) {
				strPersent.Format("%d%%", i+1);
				StaticPersent->SetWindowText(strPersent);
				ProgressBar->StepIt();
				Sleep(10);
			}
			StaticMessage->SetWindowText(Message);
		}
	}
}


// 저장될 데이터와 HTML을 분리해 파싱된 HTML테그를 만듦.
UINT ManagerDlgSavePostThread(LPVOID lParam) {

	ManagerDlgSetStateMessage(lParam, MSG_PREPAREWORK, 0, 0);
	CHtmlParser htmlParser(
		ManagerDlgCurrentLocation, 
		ManagerDlgOrgContents);	// 데이터 바인딩

	ManagerDlgSetStateMessage(lParam, MSG_PREPAREPARSE, 0, 10);
	ManagerDlgSetStateMessage(lParam, MSG_PREPAREWORKPARSE, 10, 30);
	htmlParser.GetParseOfContents(); // 파싱 작업
	ManagerDlgSetStateMessage(lParam, MSG_COMPLETEWORKPARSE, 30, 70);
	ManagerDlgSetStateMessage(lParam, MSG_PREPAREHTTPCONNECT, 70, 80);

	CHttpWrapper httpWrapper;
	bool bSuccess = false;
	if(!httpWrapper.SetSavePost(
		ManagerDlgUserID,
		ManagerDlgSubject,
		htmlParser.PostStaticData, htmlParser.PostContents)) {
		ManagerDlgSetStateMessage(lParam, MSG_COMPLETEFILUREWORK + httpWrapper.GetLastError(), 80, 100);

	}else {
		ManagerDlgSetStateMessage(lParam, MSG_COMPLETESUCCESSWORK, 80, 100);
		bSuccess = true;
	}
	bManagerDlgExistStateThread = false;
	ManagerDlgThreadEnd(lParam, bSuccess);
	return 0;
}

// 저장될 데이터와 HTML을 분리하는 작업하지 않는 쓰레드.
UINT ManagerDlgSavePostThreadEx(LPVOID lParam) {

	ManagerDlgSetStateMessage(lParam, MSG_PREPAREWORK, 0, 0);
	CHtmlParser htmlParser(
		ManagerDlgCurrentLocation, 
		ManagerDlgOrgContents);	// 데이터 바인딩

	ManagerDlgSetStateMessage(lParam, MSG_PREPAREPARSE, 0, 10);
	ManagerDlgSetStateMessage(lParam, MSG_PREPAREWORKPARSE, 10, 30);

	htmlParser.GetParseOfContents(FALSE);
	ManagerDlgSetStateMessage(lParam, MSG_COMPLETEWORKPARSE, 30, 70);
	ManagerDlgSetStateMessage(lParam, MSG_PREPAREHTTPCONNECT, 70, 80);

	CHttpWrapper httpWrapper;
	bool bSuccess = false;
	if(!httpWrapper.SetSavePost(
		ManagerDlgUserID, 
		ManagerDlgSubject,
		htmlParser.PostStaticData, htmlParser.PostContents)) {
		ManagerDlgSetStateMessage(lParam, MSG_COMPLETEFILUREWORK + httpWrapper.GetLastError(), 80, 100);

	}else {
		ManagerDlgSetStateMessage(lParam, MSG_COMPLETESUCCESSWORK, 80, 100);
		bSuccess = true;
	}
	bManagerDlgExistStateThread = false;
	ManagerDlgThreadEnd(lParam, bSuccess);
	return 0;
}

CManagerDlg::CManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManagerDlg)
	m_strContents = _T("");
	m_strLocation = _T("");
	//}}AFX_DATA_INIT
	
	bShowMessage		= false;
	m_configuration		= new CConfiguration();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONMANAGEDLG);
}

CManagerDlg::~CManagerDlg() {
	delete m_configuration;
}

void CManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManagerDlg)
	DDX_Control(pDX, IDC_CHKTAGPARSE, m_ctrlTagParse);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProbressBar);
	DDX_Text(pDX, IDC_TXTORGCONTETNS, m_strContents);
	DDX_Text(pDX, IDC_TXTCURRENTLOCATION, m_strLocation);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ANIMATE, m_ctrlAnimate);
	DDX_Control(pDX, IDC_CHKAUTOCLOSE, m_ctrlAutoClose);
}


BEGIN_MESSAGE_MAP(CManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CManagerDlg)
	ON_BN_CLICKED(IDC_BTNSAVEPOST, OnBtnSavePost)
	//}}AFX_MSG_MAP
//	ON_WM_MOVE()
//ON_ACN_START(IDC_ANIMATE, &CManagerDlg::OnAcnStartAnimate)
ON_BN_CLICKED(IDCANCEL, &CManagerDlg::OnBnClickedCancel)
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_SHOWMESSAGE, &CManagerDlg::OnBnClickedShowmessage)
ON_BN_CLICKED(IDC_BTNOPTION, &CManagerDlg::OnBnClickedBtnoption)
ON_BN_CLICKED(IDC_BTNCOPYCLIPBOARD, &CManagerDlg::OnBnClickedBtncopyclipboard)
ON_BN_CLICKED(IDC_SAVETOLOCAL, &CManagerDlg::OnBnClickedSavetolocal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManagerDlg message handlers

BOOL CManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlProbressBar.SetRange(0, 100);
	m_ctrlProbressBar.SetStep(1);
	m_ctrlProbressBar.SetPos(0);

	m_ctrlTagParse.SetCheck(TRUE);
	m_ctrlAnimate.Open(IDR_ANIDOWNLOAD);
	m_ctrlAutoClose.SetCheck(TRUE);

	SetManagerDlgWindowSize(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManagerDlg::SetManagerDlgWindowSize(BOOL bReduce) {
	CRect rCurrentWindow;
	GetWindowRect(rCurrentWindow);
	int iWindowSize = (bReduce) ? MANAGEDLG_REDUCE_SIZE : MANAGEDLG_COLLAPSE_SIZE;
	MoveWindow(rCurrentWindow.left, rCurrentWindow.top, rCurrentWindow.Width(), iWindowSize);
}

void CManagerDlg::OnBtnSavePost() 
{
	UpdateData(TRUE);

	m_ctrlProbressBar.SetPos(0);
	ManagerDlgUserID			= m_configuration->GetLoginUserID();
	ManagerDlgSubject			= m_strLocation;
	ManagerDlgCurrentLocation	= m_strOrgLocation;
	ManagerDlgOrgContents		= m_strContents;

	bManagerDlgExistStateThread = true;

	CButton *BtnSavePost = (CButton*)GetDlgItem(IDC_BTNSAVEPOST);
	CButton *BtnCancel = (CButton*)GetDlgItem(IDCANCEL);
	BtnSavePost->EnableWindow(FALSE);
	BtnCancel->EnableWindow(FALSE);

	m_ctrlAnimate.Play(0, -1, -1);
	if(m_ctrlTagParse.GetCheck() == TRUE) {
		AfxBeginThread(ManagerDlgSavePostThread, (LPVOID)this);
	}else {
		AfxBeginThread(ManagerDlgSavePostThreadEx, (LPVOID)this);
	}
}

void CManagerDlg::OnShowMessage() 
{
	CButton *btnShowMessage = (CButton*)GetDlgItem(IDC_SHOWMESSAGE);
	if(!bShowMessage) {
		btnShowMessage->SetWindowText("간단히 ▲");	
		SetManagerDlgWindowSize(FALSE);
		
	}else {
		btnShowMessage->SetWindowText("자세히 ▼");	
		SetManagerDlgWindowSize(TRUE);
	}

	bShowMessage = !bShowMessage;
}


void CManagerDlg::SetStateMessage(CString Message)
{
	CStatic *StaticMessage = (CStatic*)GetDlgItem(IDC_STATICMESSAGE);
	StaticMessage->SetWindowText(Message);
	UpdateData(FALSE);
}

void CManagerDlg::OnBnClickedCancel()
{
	if(!bManagerDlgExistStateThread)
		OnCancel();
}

void CManagerDlg::OnDestroy()
{
	if(!bManagerDlgExistStateThread)
		CDialog::OnDestroy();
}

void CManagerDlg::OnBnClickedShowmessage()
{
	OnShowMessage();
}

void CManagerDlg::OnBnClickedBtnoption()
{
	CConfigurationDlg *m_configurationDlg = new CConfigurationDlg();
	m_configurationDlg->Create(IDD_CONFIGURATIONDLG, this);
	m_configurationDlg->ShowWindow(SW_SHOW);
}

void CManagerDlg::OnBnClickedBtncopyclipboard()
{
	UpdateData(TRUE);
	OpenClipboard();
	EmptyClipboard();

	int iLength = m_strContents.GetLength()+1;
	HGLOBAL hResult = GlobalAlloc(GHND, iLength);
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hResult);          
	memcpy(lptstrCopy, m_strContents.GetBuffer(), iLength);     

	GlobalUnlock(hResult);
	SetClipboardData(CF_TEXT, hResult);  
	CloseClipboard();
	AfxMessageBox(MANAGERDLG_CLIPBOARD_COMPLETE);
}

void CManagerDlg::OnBnClickedSavetolocal()
{
	UpdateData(TRUE);
	CFileDialog dlg(FALSE, NULL, NULL,
             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			 "MFM Html 스크랩 파일 (*.html)|*.html|MFM Text 스크랩 파일 (*.txt)|*.txt||");
	dlg.m_ofn.lpstrDefExt = "html";

	if(dlg.DoModal() == IDOK) {
		CFile SaveFile;
		if(SaveFile.Open(dlg.GetPathName(),
			CFile::modeCreate | CFile::modeReadWrite)) {

				CHtmlParser htmlParser(m_strOrgLocation, m_strContents);
				htmlParser.GetParseOfContents(FALSE);
				CString strContents = htmlParser.PostContents;

				SaveFile.Write(strContents, strContents.GetLength());
				SaveFile.Close();

				AfxMessageBox(MANAGERDLG_SAVEFILE_COMPLETE);
		}
	}
}

void CManagerDlg::SetContents(CString Contents)
{
	m_strOrgContents	= Contents;
	m_strContents		= Contents;
}

void CManagerDlg::SetLocation(CString Location)
{
	m_strOrgLocation	= Location;
	m_strLocation		= Location;
}
