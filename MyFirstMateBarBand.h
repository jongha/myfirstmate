// MyFirstMateBarBand.h : Declaration of the CMyFirstMateBarBand

#ifndef __MyFirstMateBarBAND_H_
#define __MyFirstMateBarBAND_H_

#include "resource.h"       // main symbols
#include "NavigateToolBarCtrl.h"


#define TB_MIN_SIZE_X				10
#define TB_MIN_SIZE_Y				25
#define TB_MAX_SIZE_Y				25

/////////////////////////////////////////////////////////////////////////////
// CMyFirstMateBarBand
class ATL_NO_VTABLE CMyFirstMateBarBand : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyFirstMateBarBand, &CLSID_MyFirstMateBarBand>,
	public IObjectWithSiteImpl<CMyFirstMateBarBand>,
	public IDispatchImpl<IMyFirstMateBarBand, &IID_IMyFirstMateBarBand, &LIBID_MyFirstMateBarLib>,
	public IInputObject,
	public IDeskBand
{
public:
	CMyFirstMateBarBand()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MyFirstMateBarBAND)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMyFirstMateBarBand)
	COM_INTERFACE_ENTRY(IMyFirstMateBarBand)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IInputObject)
	COM_INTERFACE_ENTRY(IDeskBand)
	COM_INTERFACE_ENTRY2(IOleWindow, IDeskBand)
	COM_INTERFACE_ENTRY2(IDockingWindow, IDeskBand)
END_COM_MAP()

// Interfaces
public:
	IWebBrowser2Ptr m_pIE;

	//IOleWindow methods
	STDMETHOD (GetWindow) (HWND*);
	STDMETHOD (ContextSensitiveHelp) (BOOL);

	//IDockingWindow methods
	STDMETHOD (ShowDW) (BOOL fShow);
	STDMETHOD (CloseDW) (DWORD dwReserved);
	STDMETHOD (ResizeBorderDW) (LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);

	//IDeskBand methods
	STDMETHOD (GetBandInfo) (DWORD, DWORD, DESKBANDINFO*);

	//IInputObject methods
	STDMETHOD (UIActivateIO) (BOOL, LPMSG);
	STDMETHOD (HasFocusIO) (void);
	STDMETHOD (TranslateAcceleratorIO) (LPMSG);

	//IObjectWithSite methods
	STDMETHOD (SetSite) (IUnknown*);
	STDMETHOD (GetSite) (REFIID, LPVOID*);


// Implementation:
public:
	void FocusChange(bool bFocus);
	bool CreateToolWindow(void);

	bool m_bFocus;		
	IInputObjectSitePtr m_pIOSite;
	HWND m_hWndParent;
	DWORD m_dwBandID;
	DWORD m_dwViewMode;

	CNavigateToolBarCtrl m_wndToolBar;
	CWnd m_wndReflectionWnd;
	
public:
	int ShowLogoutDlg();
	int ShowLoginDlg();
	long NavigateHomepage();
	int ShowConfigurationDlg();
	int ShowAboutDlg();

	// Outer Methods
	STDMETHOD(SetSavePost)(BSTR Location, BSTR Contents);
};

#endif //__MyFirstMateBarBAND_H_
