// LogoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "LogoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogoutDlg dialog


CLogoutDlg::CLogoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogoutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogoutDlg)
	m_strLoginMessage = _T("");
	//}}AFX_DATA_INIT

	configuration = new CConfiguration();
	m_strLoginMessage.Format(LOGIN_INFO_MESSAGE, configuration->GetLoginUserID());
}

CLogoutDlg::~CLogoutDlg() {
	delete configuration;
}

void CLogoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogoutDlg)
	DDX_Text(pDX, IDC_LOGINMESSAGE, m_strLoginMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogoutDlg, CDialog)
	//{{AFX_MSG_MAP(CLogoutDlg)
	ON_BN_CLICKED(IDC_BTNLOGOUT, OnBtnLogout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogoutDlg message handlers

void CLogoutDlg::OnBtnLogout() 
{
	configuration->SetLoginUserID("");
	OnOK();
}
