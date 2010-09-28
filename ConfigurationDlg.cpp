// ConfigurationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "ConfigurationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigurationDlg dialog


CConfigurationDlg::CConfigurationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurationDlg::IDD, pParent)
{
	//{{AFX_DATA_`(CConfigurationDlg)
	//}}AFX_DATA_INIT

	m_configuration				= new CConfiguration();
	m_configurationNetworkDlg	= new CConfigurationNetworkDlg();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONOPTIONDLG);
}

CConfigurationDlg::~CConfigurationDlg() {
	delete m_configurationNetworkDlg;
	delete m_configuration;
}

void CConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigurationDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TABPANEL, m_ctrlPane);
	DDX_Control(pDX, IDC_CONTROLPLACEHOLDER, m_ctrlPlaceHolder);
}


BEGIN_MESSAGE_MAP(CConfigurationDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigurationDlg)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTNSETDEFAULT, OnBtnSetDefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABPANEL, &CConfigurationDlg::OnTcnSelchangeTabpanel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigurationDlg message handlers

void CConfigurationDlg::OnBtnSave() 
{
	if(UpdateData(TRUE)) {
		m_configurationNetworkDlg->SetSettings();
	}
	OnOK();
}

void CConfigurationDlg::OnBtnSetDefault() 
{
	if(AfxMessageBox(CONFIGURATION_SETDEFAULT_CONFIRM, MB_YESNO|MB_ICONWARNING) 
		== IDYES) {
		m_configuration->InitializeConfiguration();
		m_configurationNetworkDlg->GetConfiguration();
	}
}

BOOL CConfigurationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlPane.InsertItem(0, _T("네트웍"));
	//m_ctrlPane.InsertItem(1, _T("보안"));
	//m_ctrlPane.InsertItem(2, _T("개인정보"));

	m_configurationNetworkDlg->Create(IDD_CONFIGURATIONNETWORKDLG, &m_ctrlPlaceHolder);
	m_configurationNetworkDlg->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigurationDlg::SetInitDlgItem() {
	m_configurationNetworkDlg->ShowWindow(SW_HIDE);
}

void CConfigurationDlg::OnTcnSelchangeTabpanel(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetInitDlgItem();
	switch(m_ctrlPane.GetCurSel()) {
		case 0: m_configurationNetworkDlg->ShowWindow(SW_SHOW);	break;
	}
	*pResult = 0;
}