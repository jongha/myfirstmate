// Configuration.h: interface for the CConfiguration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGURATION_H__9078292A_27E9_42B1_8C56_09891DEC897B__INCLUDED_)
#define AFX_CONFIGURATION_H__9078292A_27E9_42B1_8C56_09891DEC897B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "Registry.h"
#endif // _MSC_VER > 1000

#define CONFIG_REGISTRY_KEY			"Software\\Onpis\\MFMToolBar\\Settings"
#define DEFAULT_CONFIG_DOMAIN		"mfm.onpis.com"
#define DEFAULT_CONFIG_HOMEPAGE		"http://www.onpis.com"
#define DEFAULT_CONFIG_OPERATIONURI	"/operator.php"
#define DEFAULT_CONFIG_PORT			80
#define DEFAULT_CONFIG_SEARCHURI	"http://www.google.co.kr/search?hl=ko&q=%s"

class CConfiguration  
{
public:
	void InitializeConfiguration();
	CConfiguration();
	virtual ~CConfiguration();

	CString GetHomepage();
	CString GetDomain();
	int GetPort();
	int GetIsInitialize();
	CString GetOperatorUri();
	CString GetSearchUri();
	CString GetLoginUserID();

	BOOL SetHomepage(CString Homepage=DEFAULT_CONFIG_HOMEPAGE);
	BOOL SetDomain(CString Domain=DEFAULT_CONFIG_DOMAIN);
	BOOL SetPort(int Port=DEFAULT_CONFIG_PORT);
	BOOL SetOperatorUri(CString OperationUri=DEFAULT_CONFIG_OPERATIONURI);
	BOOL SetSearchUri(CString SearchUri=DEFAULT_CONFIG_SEARCHURI);
	BOOL SetLoginUserID(CString LoginUserID);
	BOOL SetIsInitialize(int Initialize=1);

private:
	CRegistry m_Registry;
};

#endif // !defined(AFX_CONFIGURATION_H__9078292A_27E9_42B1_8C56_09891DEC897B__INCLUDED_)
