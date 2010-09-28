// ConfigurationNetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigurationNetworkDlg.h"


// CConfigurationNetworkDlg dialog

IMPLEMENT_DYNAMIC(CConfigurationNetworkDlg, CDialog)

CConfigurationNetworkDlg::CConfigurationNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurationNetworkDlg::IDD, pParent)
	, m_strHomepage(_T(""))
	, m_strDomain(_T(""))
	, m_iPort(0)
	, m_strOperationUri(_T(""))
	, m_strSearchUri(_T(""))
{
	m_configuration = new CConfiguration();
}

CConfigurationNetworkDlg::~CConfigurationNetworkDlg()
{
	delete m_configuration;
}

void CConfigurationNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXTHOMEPAGE, m_strHomepage);
	DDX_Text(pDX, IDC_TXTDOMAIN, m_strDomain);
	DDX_Text(pDX, IDC_TXTPORT, m_iPort);
	DDX_Text(pDX, IDC_TXTOPERATIONURI, m_strOperationUri);
	DDX_Text(pDX, IDC_TXTSEARCHURI, m_strSearchUri);
}


BEGIN_MESSAGE_MAP(CConfigurationNetworkDlg, CDialog)
END_MESSAGE_MAP()


// CConfigurationNetworkDlg message handlers

void CConfigurationNetworkDlg::GetConfiguration()
{
	m_strHomepage		= m_configuration->GetHomepage();
	m_strDomain			= m_configuration->GetDomain();
	m_iPort				= m_configuration->GetPort();
	m_strOperationUri	= m_configuration->GetOperatorUri();
	m_strSearchUri		= m_configuration->GetSearchUri();
	UpdateData(FALSE);
}

BOOL CConfigurationNetworkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetConfiguration();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConfigurationNetworkDlg::SetSettings() {
	return UpdateData(TRUE) &&
		m_configuration->SetHomepage(m_strHomepage) &&
		m_configuration->SetDomain(m_strDomain) &&
		m_configuration->SetPort(m_iPort) &&
		m_configuration->SetOperatorUri(m_strOperationUri) &&
		m_configuration->SetSearchUri(m_strSearchUri);
}
BOOL CConfigurationNetworkDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN && 
		(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
        return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}
