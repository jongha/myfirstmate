#if !defined(AFX_SearchComboBOX_H__EECD939F_3AC7_42BD_8D48_5D02B528691A__INCLUDED_)
#define AFX_SearchComboBOX_H__EECD939F_3AC7_42BD_8D48_5D02B528691A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchComboBox.h : header file
//

class CMyFirstMateBarBand;

/////////////////////////////////////////////////////////////////////////////
// CSearchComboBox window

class CSearchComboBox : public CComboBoxEx
{
// Construction
public:
	CSearchComboBox();

// Attributes
public:
	CMyFirstMateBarBand* m_pBand;

// Operations
public:
	STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_ImageList;
	bool NavigateToSearch(CString SearchText);
	virtual ~CSearchComboBox();



	// Generated message map functions
protected:
	//{{AFX_MSG(CSearchComboBox)
	afx_msg void OnEditBegin( NMHDR * pNotifyStruct, LRESULT* result );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCloseup();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SearchComboBOX_H__EECD939F_3AC7_42BD_8D48_5D02B528691A__INCLUDED_)
