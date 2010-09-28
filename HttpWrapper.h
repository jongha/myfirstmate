// HttpWrapper.h: interface for the CHttpWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPWRAPPER_H__731B8227_6DE4_443B_B72A_27AE5E75EBC1__INCLUDED_)
#define AFX_HTTPWRAPPER_H__731B8227_6DE4_443B_B72A_27AE5E75EBC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "w3c.h"
#include "Configuration.h"
#endif // _MSC_VER > 1000

#define HTTP_RESULT_SUCCESS			"1"
#define HTTP_RESULT_FAIL			"0"
#define HTTP_RESULT_BUFFER_SIZE		1024

#define HTTP_REQUEST_TYPE			"type"
#define HTTP_REQUEST_ACTION			"action"
#define HTTP_REQUEST_USERID			"UserID"
#define HTTP_REQUEST_PASSWORD		"Password"
#define HTTP_REQUEST_STATICDATA		"StaticData"
#define HTTP_REQUEST_CONTENTS		"Contents"
#define HTTP_REQUEST_SUBJECT		"Subject"

#define HTTP_OPERATION_OPERATION	"Operation"
#define HTTP_OPERATION_LOGIN		"Login"
#define HTTP_OPERATION_LOGOUT		"LogOut"
#define HTTP_OPERATION_SAVEPOST		"SavePost"
#define HTTP_OPERATION_CHECKSESSION	"CheckSession"

class CHttpWrapper  
{
public:
	bool SetLogin(CString UserID, CString Password);
	bool CheckSession();
	bool SetLogout(CString UserID);
	bool SetSavePost(CString UserID, CString Subject, CString StaticData, CString Contents);
	CString GetLastError();
	CHttpWrapper();
	virtual ~CHttpWrapper();

private:
	void InitHttpWrapper();
	bool IsSuccess(CString Result);
	CString m_strMessage;
	CCommon *m_common;
	CConfiguration *m_configuration;
};

#endif // !defined(AFX_HTTPWRAPPER_H__731B8227_6DE4_443B_B72A_27AE5E75EBC1__INCLUDED_)
