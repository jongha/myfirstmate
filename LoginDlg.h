#if !defined(AFX_LOGINDLG_H__26D9F092_B6CC_40E5_956A_3D9D7AC20300__INCLUDED_)
#define AFX_LOGINDLG_H__26D9F092_B6CC_40E5_956A_3D9D7AC20300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "resource.h"
#include "HttpWrapper.h"
#include "Configuration.h"
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	~CLoginDlg();
	bool m_bLoginState;
	void SetLoginState(bool bLogin);

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGINDLG };
	CString	m_strUserID;
	CString	m_strPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	afx_msg void OnBtnLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
	CHttpWrapper *httpWrapper;
	CConfiguration *configuration;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ctrlLoginProgress;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__26D9F092_B6CC_40E5_956A_3D9D7AC20300__INCLUDED_)
#include "afxcmn.h"
