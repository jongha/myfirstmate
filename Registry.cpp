

#include "stdafx.h"
#include <winreg.h>
#include "Registry.h"

#define CLASS_NAME_LENGTH 255

CRegistry::CRegistry()
{
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	m_nLastError = ERROR_SUCCESS;
}

CRegistry::~CRegistry()
{
	ClearKey();
}


BOOL CRegistry::ClearKey()
{
	m_strCurrentPath.Empty();
	m_hRootKey = HKEY_CURRENT_USER;
	m_bLazyWrite = TRUE;
	return TRUE;
}

BOOL CRegistry::SetRootKey(HKEY hRootKey)
{
	if (hRootKey != HKEY_CLASSES_ROOT &&
			hRootKey != HKEY_CURRENT_USER &&
			hRootKey != HKEY_LOCAL_MACHINE &&
			hRootKey != HKEY_USERS) return FALSE;

	m_hRootKey = hRootKey;
	return TRUE;
}


BOOL CRegistry::CreateKey(CString strKey)
{
	ASSERT(strKey[0] != '\\');
	HKEY hKey;

	DWORD dwDisposition = 0;

	if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(strKey), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
			&dwDisposition)	!= ERROR_SUCCESS) return FALSE;
	
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	m_strCurrentPath = strKey;
	return TRUE;
}


BOOL CRegistry::DeleteKey(CString strKey)
{
	ASSERT(FALSE);
	ASSERT(strKey[0] != '\\');

	if (!KeyExists(strKey)) return TRUE;
	if (::RegDeleteKey(m_hRootKey, strKey) != ERROR_SUCCESS) return FALSE;
	return TRUE;
}



BOOL CRegistry::DeleteValue(CString strName)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	HKEY hKey;
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegDeleteValue(hKey, LPCTSTR(strName));
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}


int CRegistry::GetDataSize(CString strValueName)
{
	HKEY hKey;
	ASSERT(m_strCurrentPath.GetLength() > 0);
	LONG lResult;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) return -1;

	DWORD dwSize = 1;
	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strValueName),
		NULL, NULL, NULL, &dwSize);
	::RegCloseKey(hKey);

	if (lResult != ERROR_SUCCESS) return -1;
	return (int)dwSize;
}

DWORD CRegistry::GetDataType(CString strValueName)
{
	HKEY hKey;
	ASSERT(m_strCurrentPath.GetLength() > 0);

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_QUERY_VALUE, &hKey);

	if (m_nLastError != ERROR_SUCCESS) return 0;

	DWORD dwType = 1;
	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strValueName),
		NULL, &dwType, NULL, NULL);
	::RegCloseKey(hKey);		

	if (m_nLastError == ERROR_SUCCESS) return dwType;

	return 0;
}



int CRegistry::GetSubKeyCount()
{
	HKEY hKey;
	ASSERT(m_strCurrentPath.GetLength() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwSubKeyCount;
}


int CRegistry::GetValueCount()
{
	HKEY hKey;
	ASSERT(m_strCurrentPath.GetLength() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return -1;

	LONG lResult;
	DWORD dwSubKeyCount, dwValueCount, dwClassNameLength,
		dwMaxSubKeyName, dwMaxValueName, dwMaxValueLength;
	FILETIME ftLastWritten;

	_TCHAR szClassBuffer[CLASS_NAME_LENGTH];
		
	dwClassNameLength = CLASS_NAME_LENGTH;
	lResult = ::RegQueryInfoKey(hKey, szClassBuffer, &dwClassNameLength,
		NULL, &dwSubKeyCount, &dwMaxSubKeyName, NULL, &dwValueCount,
		&dwMaxValueName, &dwMaxValueLength, NULL, &ftLastWritten);
				
	::RegCloseKey(hKey);
	if (lResult != ERROR_SUCCESS) return -1;

	return (int)dwValueCount;
}


BOOL CRegistry::KeyExists(CString strKey, HKEY hRootKey)
{
	ASSERT(strKey[0] != '\\');
	HKEY hKey;

	if (hRootKey == NULL) hRootKey = m_hRootKey;
	
	LONG lResult = ::RegOpenKeyEx(hRootKey, LPCTSTR(strKey), 0,
		KEY_ALL_ACCESS, &hKey);
	::RegCloseKey(hKey);
	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}

BOOL CRegistry::SetKey(CString strKey, BOOL bCanCreate)
{
	ASSERT(strKey[0] != '\\');
	HKEY hKey;

	if (strKey.GetLength() == 0) {
		m_strCurrentPath.Empty();
		return TRUE;
	}

	DWORD dwDisposition;
	if (bCanCreate) {
		if (::RegCreateKeyEx(m_hRootKey, LPCTSTR(strKey), 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
				&dwDisposition) != ERROR_SUCCESS) return FALSE;
		m_strCurrentPath = strKey;
		if (!m_bLazyWrite) ::RegFlushKey(hKey);
		::RegCloseKey(hKey);	
		return TRUE;
	}

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(strKey), 0,
		KEY_ALL_ACCESS, &hKey);
	if (m_nLastError != ERROR_SUCCESS) return FALSE;
	m_strCurrentPath = strKey;
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return TRUE;
}


BOOL CRegistry::ValueExists(CString strName)
{
	HKEY hKey;
	LONG lResult;
	ASSERT(m_strCurrentPath.GetLength() > 0);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS) return FALSE;

	lResult = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		NULL, NULL, NULL);
	::RegCloseKey(hKey);

	if (lResult == ERROR_SUCCESS) return TRUE;
	return FALSE;
}


void CRegistry::RenameValue(CString strOldName, CString strNewName)
{
	ASSERT(FALSE);
}




COleDateTime CRegistry::ReadDateTime(CString strName, COleDateTime dtDefault)
{
	DWORD dwType = REG_BINARY;
	COleDateTime dt;
	DWORD dwSize = sizeof(dt);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return dtDefault;
	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&dt, &dwSize) != ERROR_SUCCESS) dt = dtDefault;
	::RegCloseKey(hKey);	
	return dt;
}


double CRegistry::ReadFloat(CString strName, double fDefault)
{
	DWORD dwType = REG_BINARY;
	double d;
	DWORD dwSize = sizeof(d);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return fDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&d, &dwSize) != ERROR_SUCCESS) d = fDefault;
	::RegCloseKey(hKey);	
	return d;
}

CString CRegistry::ReadString(CString strName, CString strDefault)
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	BOOL bSuccess = TRUE;
	_TCHAR sz[255];
	HKEY hKey;
	
	ASSERT(m_strCurrentPath.GetLength() > 0);
	dwType = GetDataType(strName);
	
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ) {
		return strDefault;
	}

	m_nLastError = ::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey);
	if (m_nLastError != ERROR_SUCCESS) return strDefault;

	m_nLastError = ::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)sz, &dwSize);
	if (m_nLastError != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	
	if (!bSuccess) return strDefault;
	return CString((LPCTSTR)sz);
}

DWORD CRegistry::ReadDword(CString strName, DWORD dwDefault)
{
	DWORD dwType = REG_DWORD;
	DWORD dw;
	DWORD dwSize = sizeof(dw);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return dwDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&dw, &dwSize) != ERROR_SUCCESS) dw = dwDefault;
	::RegCloseKey(hKey);	
	return dw;
}



int CRegistry::ReadInt(CString strName, int nDefault)
{
	DWORD dwType = REG_BINARY;
	int n;
	DWORD dwSize = sizeof(n);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return nDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&n, &dwSize) != ERROR_SUCCESS) n = nDefault;
	::RegCloseKey(hKey);	
	return n;
}

BOOL CRegistry::ReadBool(CString strName, BOOL bDefault)
{
	DWORD dwType = REG_BINARY;
	BOOL b;
	DWORD dwSize = sizeof(b);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return bDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&b, &dwSize) != ERROR_SUCCESS) b = bDefault;
	::RegCloseKey(hKey);	
	return b;
}


COLORREF CRegistry::ReadColor(CString strName, COLORREF rgbDefault)
{
	DWORD dwType = REG_BINARY;
	COLORREF rgb;
	DWORD dwSize = sizeof(rgb);
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return rgbDefault;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&rgb, &dwSize) != ERROR_SUCCESS) rgb = rgbDefault;
	::RegCloseKey(hKey);	
	return rgb;
}

BOOL CRegistry::ReadFont(CString strName, CFont* pFont)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(LOGFONT);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	LOGFONT lf;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)&lf, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	if (bSuccess)
	{
		pFont->Detach();
		pFont->CreateFontIndirect(&lf);
	}
	return bSuccess;
}


BOOL CRegistry::ReadPoint(CString strName, CPoint* pPoint)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CPoint);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)pPoint, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}

BOOL CRegistry::ReadSize(CString strName, CSize* pSize)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CSize);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)pSize, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}

BOOL CRegistry::ReadRect(CString strName, CRect* pRect)
{
	DWORD dwType = REG_BINARY;
	DWORD dwSize = sizeof(CRect);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	ASSERT(m_strCurrentPath.GetLength() > 0);
	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_READ, &hKey) != ERROR_SUCCESS) return FALSE;

	if (::RegQueryValueEx(hKey, LPCTSTR(strName), NULL,
		&dwType, (LPBYTE)pRect, &dwSize) != ERROR_SUCCESS) bSuccess = FALSE;
	::RegCloseKey(hKey);	
	return bSuccess;
}

BOOL CRegistry::WriteBool(CString strName, BOOL bValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&bValue, sizeof(bValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteDateTime(CString strName, COleDateTime dtValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&dtValue, sizeof(dtValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteString(CString strName, CString strValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	_TCHAR sz[255];
	
	if (strValue.GetLength() > 254) return FALSE;

#ifdef _UNICODE
	wstrcpy(sz, LPCTSTR(strValue));
#else
	strcpy(sz, LPCTSTR(strValue));
#endif

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
#ifdef _UNICODE
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_SZ, (LPBYTE)sz, wstrlen(sz) + 1)
		 != ERROR_SUCCESS) bSuccess = FALSE;
#else
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_SZ, (LPBYTE)sz, strlen(sz) + 1)
		 != ERROR_SUCCESS) bSuccess = FALSE;
#endif
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteFloat(CString strName, double fValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&fValue, sizeof(fValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteInt(CString strName, int nValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&nValue, sizeof(nValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteDword(CString strName, DWORD dwValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&dwValue, sizeof(dwValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteColor(CString strName, COLORREF rgbValue)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&rgbValue, sizeof(rgbValue))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteFont(CString strName, CFont* pFont)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)&lf, sizeof(lf))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WritePoint(CString strName, CPoint* pPoint)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)pPoint, sizeof(CPoint))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}


BOOL CRegistry::WriteSize(CString strName, CSize* pSize)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)pSize, sizeof(CSize))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

BOOL CRegistry::WriteRect(CString strName, CRect* pRect)
{
	ASSERT(m_strCurrentPath.GetLength() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if (::RegOpenKeyEx(m_hRootKey, LPCTSTR(m_strCurrentPath), 0,
		KEY_WRITE, &hKey) != ERROR_SUCCESS) return FALSE;
	
	if (::RegSetValueEx(hKey, LPCTSTR(strName), 0,
		REG_BINARY, (LPBYTE)pRect, sizeof(CRect))
		 != ERROR_SUCCESS) bSuccess = FALSE;
		
	if (!m_bLazyWrite) ::RegFlushKey(hKey);
	::RegCloseKey(hKey);
	return bSuccess;
}

