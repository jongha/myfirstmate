// SearchComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SearchComboBox.h"

#include "MyFirstMateBar.h"
#include "MyFirstMateBarBand.h"
#include "Configuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchComboBox

CSearchComboBox::CSearchComboBox()
{
}

CSearchComboBox::~CSearchComboBox()
{
}


BEGIN_MESSAGE_MAP(CSearchComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSearchComboBox)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(CBEN_BEGINEDIT, OnEditBegin)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchComboBox message handlers

bool CSearchComboBox::NavigateToSearch(CString SearchText)
{
	CString strURL;
	if (SearchText.IsEmpty() || SearchText == "")
		return false;

	CConfiguration configuration;
	strURL.Format(configuration.GetSearchUri(), SearchText);

	_variant_t varURL = _bstr_t(strURL);
	_variant_t varEmpty;
	m_pBand->m_pIE->Navigate2(&varURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
	return true;
}

STDMETHODIMP CSearchComboBox::TranslateAcceleratorIO(LPMSG pMsg)
{
	USES_CONVERSION;
	int nVirtKey = (int)(pMsg->wParam);
	if (WM_KEYUP == pMsg->message && VK_RETURN == nVirtKey) {
		CString strEdit;
		GetWindowText(strEdit);

		// Add item to control
		COMBOBOXEXITEM CBItem;
		memset(&CBItem, 0, sizeof(CBItem));
		CBItem.mask				= CBEIF_TEXT | CBEIF_LPARAM;// | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | ;
		CBItem.iItem			= 0;
		CBItem.pszText			= (LPTSTR)(LPCTSTR)strEdit;
		CBItem.cchTextMax		= _tcslen((LPCTSTR)strEdit);
		//CBItem.iImage			= 0; //((DWORD)searchtype * 2);
		//CBItem.iSelectedImage = CBItem.iImage + 1;
		//CBItem.lParam			= 0; //(DWORD)searchtype;

		int idx;
		if (-1 == (idx = InsertItem(&CBItem))) {
			ASSERT(FALSE);
			DWORD dwError = ::GetLastError();
		}
		NavigateToSearch(strEdit);
		return S_OK;

	}else if ((WM_KEYUP == pMsg->message || WM_KEYDOWN == pMsg->message) &&
				VK_BACK == nVirtKey || (VK_END <= nVirtKey && VK_DOWN >= nVirtKey) ||
				VK_DELETE == nVirtKey) {
		TranslateMessage(pMsg);
		DispatchMessage(pMsg);

		return S_OK;

	}else {
		return S_FALSE;
	}
}

void CSearchComboBox::OnEditBegin( NMHDR * pNotifyStruct, LRESULT* result ) 
{
	m_pBand->FocusChange(true);
	return;
}

int CSearchComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetDroppedWidth(150);
	return 0;
}

void CSearchComboBox::OnCloseup() 
{

	CComboBox* pCombo	= GetComboBoxCtrl();
	int idxSel			= pCombo->GetCurSel();

	CString strItem;
	TCHAR buff[MAX_PATH];
	COMBOBOXEXITEM CBItem;
	memset(&CBItem, 0, sizeof(CBItem));
	CBItem.mask				= CBEIF_TEXT | CBEIF_LPARAM;
	CBItem.cchTextMax		= MAX_PATH;
	CBItem.pszText			= buff;
	CBItem.iItem			= idxSel;
	CBItem.iSelectedImage	= 0;
	CBItem.lParam			= 0;

	if (!GetItem(&CBItem)) {
		ASSERT(FALSE);
		DWORD dwError = ::GetLastError();
	}
	if (-1 == CBItem.iItem)
		return;

	NavigateToSearch(buff);
}
