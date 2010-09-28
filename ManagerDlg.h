#if !defined(AFX_MANAGERDLG_H__917C3F78_654A_4B18_84B4_DDD00F24D72F__INCLUDED_)
#define AFX_MANAGERDLG_H__917C3F78_654A_4B18_84B4_DDD00F24D72F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "resource.h"
#include "Configuration.h"

#endif // _MSC_VER > 1000
// ManagerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManagerDlg dialog

class CManagerDlg : public CDialog
{
// Construction
public:
	CManagerDlg(CWnd* pParent = NULL);   // standard constructor
	~CManagerDlg();
	void SetContents(CString Contents);
	void SetLocation(CString Location);

// Dialog Data
	//{{AFX_DATA(CManagerDlg)
	enum { IDD = IDD_MFMMANAGERDLG };
	CButton	m_ctrlTagParse;
	CProgressCtrl	m_ctrlProbressBar;
	CString	m_strContents;
	CString	m_strLocation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSavePost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

private:
	CConfiguration *m_configuration;
	void SetStateMessage(CString Message);
	bool bShowMessage;
	CAnimateCtrl m_ctrlAnimate;
	CButton m_ctrlAutoClose;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	void SetManagerDlgWindowSize(BOOL bReduce);
	void OnShowMessage();
	CString	m_strOrgContents;
	CString	m_strOrgLocation;

	afx_msg void OnBnClickedShowmessage();
	afx_msg void OnBnClickedBtnoption();
	afx_msg void OnBnClickedBtncopyclipboard();
	afx_msg void OnBnClickedSavetolocal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGERDLG_H__917C3F78_654A_4B18_84B4_DDD00F24D72F__INCLUDED_)
#include "afxwin.h"
#include "afxcmn.h"
