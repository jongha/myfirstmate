// NavigateToolBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NavigateToolBarCtrl.h"

#include "MyFirstMateBar.h"
#include "MyFirstMateBarBand.h"
#include "Configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD DEFAULT_TOOLBAR_STYLE =
		WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_TABSTOP|
		TBSTYLE_TOOLTIPS|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT|TBSTYLE_LIST| // TBSTYLE_WRAPABLE |
		CCS_BOTTOM|CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE;

#define NUM_TOOLBAR_BUTTONS		7
#define COMBO_IDX				6
#define COMBO_MARGIN_TOP		2
#define COMBO_MARGIN_LEFT		0

#define COMBO_WIDTH				150
#define COMBO_HEIGHT			150

const TBBUTTON TOOLBARBUTTONS[NUM_TOOLBAR_BUTTONS];

/////////////////////////////////////////////////////////////////////////////
// CNavigateToolBarCtrl

CNavigateToolBarCtrl::CNavigateToolBarCtrl()
{
}

CNavigateToolBarCtrl::~CNavigateToolBarCtrl()
{
}

BEGIN_MESSAGE_MAP(CNavigateToolBarCtrl, CToolBarCtrl)
	//{{AFX_MSG_MAP(CNavigateToolBarCtrl)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(TBSTYLE_BUTTON, OnCommand)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, &CNavigateToolBarCtrl::OnTbnDropDown)
	ON_COMMAND(IDM_MFM_ONPISHOME, &CNavigateToolBarCtrl::OnMFMOnpisHome)
	ON_COMMAND(IDM_MFM_OPTION, &CNavigateToolBarCtrl::OnMFMOption)
	ON_COMMAND(IDM_MFM_ABOUT, &CNavigateToolBarCtrl::OnMFMAbout)
	ON_COMMAND(ID_MFM_SESSION, &CNavigateToolBarCtrl::OnMFMSession)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNavigateToolBarCtrl operations

// Default creation code
bool CNavigateToolBarCtrl::Create(CRect rcClientParent, CWnd* pWndParent, CMyFirstMateBarBand* pBand) 
{
	m_MainPopMenu.LoadMenu(IDR_MAINPOPMENU);

	if (!CToolBarCtrl::Create(DEFAULT_TOOLBAR_STYLE, 
		rcClientParent, pWndParent, IDR_IETOOLBAR))
		return false;

	SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	if (AddBitmap(1, IDR_IETOOLBAR) == -1) {
		DWORD dwError = ::GetLastError();
		return false;
	}

	TBBUTTON tbs1[2];
	TBBUTTON tbs2[7];
	SetMenuStyle1(tbs1, 2);
	SetMenuStyle2(tbs2, 7);
	if (!AddButtons(2, tbs1))
		return false;

	if (!AddButtons(7, tbs2))
		return false;

	CRect rcCombo;
	if (!m_wndCombo.Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWN, 
		rcCombo, 
		this,
		IDC_TOOLSCOMBO))
		return false;

	m_pBand				= pBand;
	m_wndCombo.m_pBand	= pBand;

	SetButtonSize();	
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CNavigateToolBarCtrl message handlers

void CNavigateToolBarCtrl::OnSize(UINT nType, int cx, int cy) 
{
	if (m_wndCombo.GetSafeHwnd()) {
		CRect rcCombo;
		CRect rcClientParent;

		rcCombo.bottom	= cy;
		rcCombo.top		= COMBO_MARGIN_TOP;

		CRect rcButton;
		GetItemRect(COMBO_IDX, &rcButton);
		
		rcCombo.left	= 80;
		rcCombo.right	= 80 + COMBO_WIDTH;
		m_wndCombo.MoveWindow(rcCombo);
	}
}

STDMETHODIMP CNavigateToolBarCtrl::TranslateAcceleratorIO(LPMSG pMsg)
{
	if (m_wndCombo.GetEditCtrl() == GetFocus() || &m_wndCombo == GetFocus())
		return m_wndCombo.TranslateAcceleratorIO(pMsg);
	
	return S_FALSE;
}

void CNavigateToolBarCtrl::OnCommand()
{
	const MSG* pMsg = GetCurrentMessage();
	CConfiguration configuration;

	int nID = LOWORD(pMsg->wParam);
	switch (nID) {
		case ID_BUTTON1:
			m_pBand->NavigateHomepage();
			break;

		case ID_BUTTON4: 
			m_pBand->ShowConfigurationDlg();
			break;

		case ID_BUTTON6:			
			m_pBand->ShowLoginDlg();
			break;

		case ID_BUTTON8: 
			m_pBand->ShowAboutDlg();
			break;
	}
}

void CNavigateToolBarCtrl::OnTbnDropDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTOOLBAR pNMTB = reinterpret_cast<LPNMTOOLBAR>(pNMHDR);

	CMenu *pSubMenu;
	bool bShowPopup = false;

	switch (pNMTB->iItem) {
		case ID_BUTTON1:
			pSubMenu	= m_MainPopMenu.GetSubMenu(0);
			bShowPopup	= true;
			break;
		default:
			return;
	}

	if(bShowPopup) {
		CRect rcToolbar;
		SendMessage(TB_GETRECT, pNMTB->iItem, (LPARAM)&rcToolbar);
		ClientToScreen(&rcToolbar);

		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, 
			rcToolbar.left, rcToolbar.bottom, this, NULL);		
	}

	*pResult = 0;
}

void CNavigateToolBarCtrl::SetMenuStyle1(LPTBBUTTON tbs, int iMaxCount) {

	for (int i = 0; i<iMaxCount; i++) {
		tbs[i].dwData		= 0;
		tbs[i].fsState		= TBSTATE_ENABLED;
		tbs[i].idCommand	= ID_BUTTON1 + i;
		tbs[i].iString		= i;

		switch(i) {
			case 0:	// go home
				tbs[i].iBitmap	= 0;
				tbs[i].fsStyle	= TBSTYLE_DROPDOWN;
				break;

			case 1:
				tbs[i].iBitmap	= 0;
				tbs[i].fsStyle	= TBSTYLE_SEP;
				break;
		}
	}

    AddStrings(_T("ONPIS\0"));
    AddStrings(_T("\0"));
}

void CNavigateToolBarCtrl::SetMenuStyle2(LPTBBUTTON tbs, int iMaxCount) {

	for (int i = 0; i<iMaxCount; i++) {
		tbs[i].dwData		= 0;
		tbs[i].fsState		= TBSTATE_ENABLED;
		tbs[i].idCommand	= ID_BUTTON1 + i + 2;
		tbs[i].iString		= i + 2;

		switch(i) {
			case 1:	// configuration
				tbs[i].iBitmap	= 1;
				tbs[i].fsStyle	= TBSTYLE_BUTTON;
				break;

			case 3:	// manage session
				tbs[i].iBitmap	= 2;
				tbs[i].fsStyle	= TBSTYLE_BUTTON;
				break;

			case 5:	// about toolbar
				tbs[i].iBitmap	= 3;
				tbs[i].fsStyle	= TBSTYLE_BUTTON;
				break;

			default:
				tbs[i].iBitmap	= 0;
				tbs[i].fsStyle	= TBSTYLE_SEP;
				break;
		}
	}

    AddStrings(_T("\0"));
    AddStrings(_T("환경설정\0"));
    AddStrings(_T("\0"));
    AddStrings(_T("세션관리\0"));
	AddStrings(_T("\0"));	
    AddStrings(_T("툴바 정보\0"));
	AddStrings(_T("\0"));	
}

void CNavigateToolBarCtrl::SetButtonSize() {

	TBBUTTONINFO tbi;			
    tbi.cbSize = sizeof(TBBUTTONINFO);		
	tbi.dwMask = TBIF_STYLE | TBIF_SIZE;
	tbi.fsStyle = TBSTYLE_SEP;
	tbi.cx = (unsigned short)COMBO_WIDTH;
	SetButtonInfo(ID_BUTTON2, &tbi); 		
	
	tbi.fsStyle = TBSTYLE_DROPDOWN;
	tbi.cx = (unsigned short)77;
	SetButtonInfo(ID_BUTTON1, &tbi); 	
}
void CNavigateToolBarCtrl::OnMFMOnpisHome()
{
	m_pBand->NavigateHomepage();
}

void CNavigateToolBarCtrl::OnMFMOption()
{
	m_pBand->ShowConfigurationDlg();
}

void CNavigateToolBarCtrl::OnMFMAbout()
{
	m_pBand->ShowAboutDlg();
}

void CNavigateToolBarCtrl::OnMFMSession()
{
	m_pBand->ShowLoginDlg();
}
