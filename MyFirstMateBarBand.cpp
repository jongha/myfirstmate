// MyFirstMateBarBand.cpp : Implementation of CMyFirstMateBarBand
#include "stdafx.h"
#include "MyFirstMateBar.h"
#include "MyFirstMateBarBand.h"
#include "ManagerDlg.h"
#include "Configuration.h"
#include "ConfigurationDlg.h"
#include "LoginDlg.h"
#include "LogoutDlg.h"
#include "AboutDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMyFirstMateBarBand

bool CMyFirstMateBarBand::CreateToolWindow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // Needed for any MFC usage in DLL
	
	CRect rcClientParent;
	CWnd* pWndParent = CWnd::FromHandle(m_hWndParent);
	pWndParent->GetClientRect(&rcClientParent);

	// We need to create a reflection window in between our toolbar control
	// and the rebar in order to get WM_COMMAND messages sent from the toolbar to its
	// parent. 
	if (!m_wndReflectionWnd.Create(NULL, NULL, WS_CHILD, rcClientParent, pWndParent, 0))
		return false;

	if (!m_wndToolBar.Create(rcClientParent, &m_wndReflectionWnd, this))
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
// IOleWindow Implementation
//

STDMETHODIMP CMyFirstMateBarBand::GetWindow(HWND *phWnd)
{
	*phWnd = m_wndToolBar.GetSafeHwnd();

	return S_OK;
}

STDMETHODIMP CMyFirstMateBarBand::ContextSensitiveHelp(BOOL fEnterMode)
{
	ATLTRACENOTIMPL("IOleWindow::ContextSensitiveHelp");
}

///////////////////////////////////////////////////////////////////////////
//
// IDockingWindow Implementation
//

STDMETHODIMP CMyFirstMateBarBand::ShowDW(BOOL fShow)
{
	if(m_wndToolBar.GetSafeHwnd())
	{
		if(fShow)
		{
			//show our window
			m_wndToolBar.ShowWindow(SW_SHOW);
		}
		else
		{
			//hide our window
			m_wndToolBar.ShowWindow(SW_HIDE);
		}
	}

	return S_OK;
}

STDMETHODIMP CMyFirstMateBarBand::CloseDW(DWORD dwReserved)
{
	ShowDW(FALSE);

	// CToolBar will auto clean up

	return S_OK;
}

STDMETHODIMP CMyFirstMateBarBand::ResizeBorderDW(   LPCRECT prcBorder, 
                                          IUnknown* punkSite, 
                                          BOOL fReserved)
{
	ATLTRACENOTIMPL("IDockingWindow::ResizeBorderDW");
}

///////////////////////////////////////////////////////////////////////////
//
// IInputObject Implementation
//

STDMETHODIMP CMyFirstMateBarBand::UIActivateIO(BOOL fActivate, LPMSG pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // Needed for any MFC usage in DLL
	TRACE(_T("UIActivateIO: %s\n"), (fActivate) ? _T("TRUE") : _T("FALSE"));

	if(fActivate)
	{
		m_wndToolBar.m_wndCombo.GetEditCtrl()->SetFocus();
		// will cause a FocusChange() to precipitate from edit control
	}

	return S_OK;
}

STDMETHODIMP CMyFirstMateBarBand::HasFocusIO(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // Needed for any MFC usage in DLL
	TRACE(_T("HasFocusIO\n"));

	HWND hwndFocus = ::GetFocus();
	HWND hwndEdit = m_wndToolBar.m_wndCombo.GetEditCtrl()->GetSafeHwnd();
	if (hwndEdit == hwndFocus)
		return S_OK;
	HWND hwndCombo = m_wndToolBar.m_wndCombo.GetSafeHwnd();
	if (hwndCombo == hwndFocus)
		return S_OK;
	BOOL bDropped = m_wndToolBar.m_wndCombo.GetDroppedState();
	if (bDropped)
		return S_OK;

	return S_FALSE;
}

STDMETHODIMP CMyFirstMateBarBand::TranslateAcceleratorIO(LPMSG pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); // Needed for any MFC usage in DLL
	TRACE(_T("toolband::TAIO -- msg = %d | vk = %d\n"), pMsg->message, (int)pMsg->wParam);

	return m_wndToolBar.TranslateAcceleratorIO(pMsg);
}

///////////////////////////////////////////////////////////////////////////
//
// IObjectWithSite implementations
//

STDMETHODIMP CMyFirstMateBarBand::SetSite(IUnknown* punkSite)
{
	HRESULT hr;
	m_pIOSite = punkSite;

	//If punkSite is not NULL, a new site is being set.
	if(m_pIOSite)
	{
		//Get the parent window.
		IOleWindowPtr pOleWindow;

		m_hWndParent = NULL;

		if (pOleWindow = m_pIOSite)
			pOleWindow->GetWindow(&m_hWndParent);

		if(!m_hWndParent)
			return E_FAIL;

		// Get the top level IE IWebBrowser 
		IServiceProviderPtr pSP = m_pIOSite;
		if (m_pIE)
			m_pIE = NULL;
		hr = pSP->QueryService(SID_SWebBrowserApp, IID_IWebBrowser, (void**)&m_pIE);
		
		if(!CreateToolWindow())
			return E_FAIL;
		
		// NOTE: CreateToolWindow call should be last, it relies on m_pIE 
	}

	return S_OK;
}

STDMETHODIMP CMyFirstMateBarBand::GetSite(REFIID riid, LPVOID *ppvReturn)
{
	*ppvReturn = NULL;

	if(m_pIOSite)
	   return m_pIOSite->QueryInterface(riid, ppvReturn);

	return E_FAIL;
}

///////////////////////////////////////////////////////////////////////////
//
// IDeskBand implementation
//

STDMETHODIMP CMyFirstMateBarBand::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
USES_CONVERSION;

	if(pdbi)
	{
		m_dwBandID = dwBandID;
		m_dwViewMode = dwViewMode;

		if(pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = TB_MIN_SIZE_X;
			pdbi->ptMinSize.y = TB_MIN_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = -1;
			pdbi->ptMaxSize.y = TB_MAX_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1;
			pdbi->ptIntegral.y = 1;
		}

		if(pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 200;
			pdbi->ptActual.y = TB_MAX_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_TITLE)
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState()); // Needed for any MFC usage in DLL
			CString strTitle((LPCSTR)IDS_TOOLBARNAME);
			lstrcpyW(pdbi->wszTitle, T2W((LPTSTR)(LPCTSTR)strTitle));
		}

		if(pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_NORMAL;

			pdbi->dwModeFlags |= DBIMF_VARIABLEHEIGHT;
		}

		if(pdbi->dwMask & DBIM_BKCOLOR)
		{
			//Use the default background color by removing this flag.
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}

		return S_OK;
	}
	else // !pdbi
		return E_INVALIDARG;
}

void CMyFirstMateBarBand::FocusChange(bool bFocus)
{
	TRACE(_T("Focus changed: %s\n"), (bFocus) ? _T("TRUE") : _T("FALSE"));
	m_bFocus = bFocus;

	IUnknownPtr pBand;
	try
	{
		QueryInterface(IID_IUnknown, (void**)&pBand);
		m_pIOSite->OnFocusChangeIS(pBand, (BOOL)bFocus);
	}
	catch(...)
	{

	}
}


STDMETHODIMP CMyFirstMateBarBand::SetSavePost(BSTR Location, BSTR Contents)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strContnets, strLocation;
	USES_CONVERSION; 
	strContnets.Format("%s", OLE2CA(Contents));
	strLocation.Format("%s", OLE2CA(Location));

	CManagerDlg *ManagerDlg		= new CManagerDlg();
	ManagerDlg->SetLocation(strLocation);
	ManagerDlg->SetContents(strContnets);
	ManagerDlg->DoModal();
	delete ManagerDlg;


	//AfxMessageBox(lpaszTemp);

/*
	//AfxMessageBox("1"); 
	CHtmlParser* parser = new CHtmlParser(Contents);
	if (parser->getDocument()) {
		BSTR Output;
		
		Output = parser->GetinnerHTML();
 
		USES_CONVERSION; 
		LPCSTR  lpaszTemp = OLE2CA(Contents); 
		//AfxMessageBox(lpaszTemp);

		//CString strMsg;
		//strMsg.Format("%s", Output);
		AfxMessageBox(lpaszTemp); 

	//	wcout.write(text.c_str(), text.length());
	}
	delete parser;
*/

	/*****************************************************

	HINSTANCE hinstMSHTML = LoadLibrary(TEXT("MSHTML.DLL"));
	if (hinstMSHTML != NULL) { 
		SHOWHTMLDIALOGFN* pfnShowHTMLDialog; 
		pfnShowHTMLDialog = (SHOWHTMLDIALOGFN*)GetProcAddress(hinstMSHTML, TEXT("ShowHTMLDialog")); 	
		if (pfnShowHTMLDialog) { 
			IMoniker *pURLMoniker; 
			BSTR bstrURL = SysAllocString(L"http://www.example.com/dialogsource.htm"); 
			CreateURLMoniker(NULL, bstrURL, &pURLMoniker); 
			if (pURLMoniker) 
			{
				(*pfnShowHTMLDialog)(NULL, pURLMoniker, NULL, NULL, NULL); 
				pURLMoniker->Release(); 
			} 
			SysFreeString(bstrURL); 
		} 
	}
	FreeLibrary(hinstMSHTML);
*/




/*

	IXMLDOMDocument2Ptr pDoc; // XML Document 선언
	IXMLDOMProcessingInstructionPtr pPI; // XML ProcessingInstruction 선언
	IXMLDOMElementPtr pRootElement; // XML Element 선언
	IXMLDOMElementPtr pElement;
	IXMLDOMTextPtr pText; // XML Text 선언

	pDoc.CreateInstance(__uuidof(DOMDocument)); // 인스턴스 생성
	pDoc->text = lpaszTemp;
	AfxMessageBox(pDoc->x); // XML 내용을 보자!!

	pPI=pDoc->createProcessingInstruction(L"xml",L"version=\"1.0\" encoding=\"euc-kr\""); // PI 생성
	pRootElement=pDoc->createElement(L"IRC"); // Root element 생성
	pElement=pDoc->createElement(L"Nick"); // "Nick" element 생성
	pText=pDoc->createTextNode(L"wiluby"); // "wiluby" text element 생성
	pDoc->appendChild(pPI); // ProcessingIsntruction 추가
	pElement->appendChild(pText); // text element를 "Nick" element에 추가
	pRootElement->appendChild(pElement); // "Nick" element를 root element에 추가
	pDoc->appendChild(pRootElement); // Root element를 문서에 추가
	AfxMessageBox(pDoc->xml); // XML 내용을 보자!!
*/
	return S_OK;
}

int CMyFirstMateBarBand::ShowConfigurationDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CConfigurationDlg *configurationDlg = new CConfigurationDlg();
	int iResult = configurationDlg->DoModal();

	delete configurationDlg;
	return iResult;
}

int CMyFirstMateBarBand::ShowAboutDlg() {

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CAboutDlg *aboutDlg = new CAboutDlg();
	int iResult = aboutDlg->DoModal();

	delete aboutDlg;
	return iResult;
}

long CMyFirstMateBarBand::NavigateHomepage()
{
	CConfiguration *configuration = new CConfiguration();
	_variant_t varEmpty;
	_variant_t varURL;
	varURL = _bstr_t(configuration->GetHomepage());
	int iResult = m_pIE->Navigate2(&varURL, 
		&varEmpty, &varEmpty, &varEmpty, &varEmpty);

	delete configuration;
	return iResult;
}

int CMyFirstMateBarBand::ShowLoginDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CLoginDlg *loginDlg = new CLoginDlg();
	int iResult = loginDlg->DoModal();

	delete loginDlg;
	return iResult;
}

int CMyFirstMateBarBand::ShowLogoutDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CLogoutDlg *logoutDlg = new CLogoutDlg();
	int iResult = logoutDlg->DoModal();

	delete logoutDlg;
	return iResult;
}
