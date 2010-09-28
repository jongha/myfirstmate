// Configuration.cpp: implementation of the CConfiguration class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFirstMateBar.h"
#include "Configuration.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfiguration::CConfiguration()
{
	m_Registry.SetRootKey(HKEY_CURRENT_USER);
	m_Registry.SetKey(CONFIG_REGISTRY_KEY, TRUE);
	if(GetIsInitialize() == FALSE) {
		InitializeConfiguration();
	}
}

CConfiguration::~CConfiguration()
{

}

int CConfiguration::GetIsInitialize() {
	return m_Registry.ReadInt("Initialize", 0);
}

BOOL CConfiguration::SetIsInitialize(int Initialize) {
	return m_Registry.WriteInt("Initialize", Initialize);
}

CString CConfiguration::GetDomain() {
	return m_Registry.ReadString("Domain", DEFAULT_CONFIG_DOMAIN);
}

BOOL CConfiguration::SetDomain(CString Domain) {
	return m_Registry.WriteString("Domain", Domain);
}

CString CConfiguration::GetHomepage() {
	return m_Registry.ReadString("Homepage", DEFAULT_CONFIG_HOMEPAGE);
}

BOOL CConfiguration::SetHomepage(CString Homepage) {
	return m_Registry.WriteString("Homepage", Homepage);
}

int CConfiguration::GetPort() {
	return m_Registry.ReadInt("Port", DEFAULT_CONFIG_PORT);
}

BOOL CConfiguration::SetPort(int Port) {
	return m_Registry.WriteInt("Port", Port);
}

CString CConfiguration::GetOperatorUri() {
	return m_Registry.ReadString("OperationUri", DEFAULT_CONFIG_OPERATIONURI);
}

BOOL CConfiguration::SetOperatorUri(CString SearchUri) {
	return m_Registry.WriteString("OperationUri", SearchUri);
}

CString CConfiguration::GetSearchUri() {
	return m_Registry.ReadString("SearchUri", DEFAULT_CONFIG_SEARCHURI);
}

BOOL CConfiguration::SetSearchUri(CString SearchUri) {
	return m_Registry.WriteString("SearchUri", SearchUri);
}

CString CConfiguration::GetLoginUserID() {
	return m_Registry.ReadString("LoginUserID", "");
}

BOOL CConfiguration::SetLoginUserID(CString LoginUserID) {
	return m_Registry.WriteString("LoginUserID", LoginUserID);
}

void CConfiguration::InitializeConfiguration()
{
	SetIsInitialize();
	SetHomepage();
	SetDomain();
	SetPort();
	SetOperatorUri();
	SetSearchUri();
}
