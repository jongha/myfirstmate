#if !defined(AFX_CONFIGURATIONDLG_H__2199148A_7F8D_46B7_B217_C69930A9347A__INCLUDED_)
#define AFX_CONFIGURATIONDLG_H__2199148A_7F8D_46B7_B217_C69930A9347A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "resource.h"
#include "configuration.h"
#include "ConfigurationNetworkDlg.h"
#endif // _MSC_VER > 1000
// ConfigurationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigurationDlg dialog

class CConfigurationDlg : public CDialog
{
// Construction
public:
	CConfigurationDlg(CWnd* pParent = NULL);   // standard constructor
	~CConfigurationDlg();

// Dialog Data
	//{{AFX_DATA(CConfigurationDlg)
	enum { IDD = IDD_CONFIGURATIONDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigurationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigurationDlg)
	afx_msg void OnBtnSave();
	afx_msg void OnBtnSetDefault();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
	CConfiguration *m_configuration;
	CConfigurationNetworkDlg *m_configurationNetworkDlg;
	CTabCtrl m_ctrlPane;
	afx_msg void OnTcnSelchangeTabpanel(NMHDR *pNMHDR, LRESULT *pResult);
	void SetInitDlgItem();
	CStatic m_ctrlPlaceHolder;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATIONDLG_H__2199148A_7F8D_46B7_B217_C69930A9347A__INCLUDED_)
#include "afxcmn.h"
#include "afxwin.h"
