#if !defined(AFX_IETOOLBARCTRL_H__92D63B35_5805_4960_9770_B455E11FF4A7__INCLUDED_)
#define AFX_IETOOLBARCTRL_H__92D63B35_5805_4960_9770_B455E11FF4A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEToolBarCtrl.h : header file
//

#include "SearchComboBox.h"

class CMyFirstMateBarBand;

/////////////////////////////////////////////////////////////////////////////
// CNavigateToolBarCtrl window

class CNavigateToolBarCtrl : public CToolBarCtrl
{
// Construction
public:
	CNavigateToolBarCtrl();

// Attributes
public:
	CMyFirstMateBarBand* m_pBand;

// Operations
public:
	bool Create(CRect rcClientParent, CWnd* pWndParent, CMyFirstMateBarBand* pBand);

	STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavigateToolBarCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CSearchComboBox m_wndCombo;
	virtual ~CNavigateToolBarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNavigateToolBarCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCommand();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void ShowPopupMenu(UINT nCmdID);
	afx_msg void OnTbnDropDown(NMHDR *pNMHDR, LRESULT *pResult);
	CMenu m_MainPopMenu;
	void SetMenuStyle1(LPTBBUTTON tbs, int iMaxCount);
	void SetMenuStyle2(LPTBBUTTON tbs, int iMaxCount);
	void SetButtonSize();
	afx_msg void OnMFMOnpisHome();
	afx_msg void OnMFMOption();
	afx_msg void OnMFMAbout();
	afx_msg void OnMFMSession();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IETOOLBARCTRL_H__92D63B35_5805_4960_9770_B455E11FF4A7__INCLUDED_)
