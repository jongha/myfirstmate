#if !defined(AFX_LOGOUTDLG_H__3F4152F2_A943_4708_9EEC_A7AF3114CB8F__INCLUDED_)
#define AFX_LOGOUTDLG_H__3F4152F2_A943_4708_9EEC_A7AF3114CB8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "resource.h"
#include "Configuration.h"
#endif // _MSC_VER > 1000
// LogoutDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogoutDlg dialog

class CLogoutDlg : public CDialog
{
// Construction
public:
	CLogoutDlg(CWnd* pParent = NULL);   // standard constructor
	~CLogoutDlg();

// Dialog Data
	//{{AFX_DATA(CLogoutDlg)
	enum { IDD = IDD_LOGOUTDLG };
	CString	m_strLoginMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogoutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogoutDlg)
	afx_msg void OnBtnLogout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CConfiguration *configuration;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGOUTDLG_H__3F4152F2_A943_4708_9EEC_A7AF3114CB8F__INCLUDED_)
