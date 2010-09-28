#pragma once

#include "resource.h"
#include "Configuration.h"

// CConfigurationNetworkDlg dialog

class CConfigurationNetworkDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigurationNetworkDlg)

public:
	CConfigurationNetworkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigurationNetworkDlg();

// Dialog Data
	enum { IDD = IDD_CONFIGURATIONNETWORKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strHomepage;
	CString m_strDomain;
	int m_iPort;
	CString m_strOperationUri;
	CString m_strSearchUri;
	void GetConfiguration();
public:
	virtual BOOL OnInitDialog();
	CConfiguration *m_configuration;
	BOOL SetSettings();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
