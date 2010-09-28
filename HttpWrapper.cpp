// HttpWrapper.cpp: implementation of the CHttpWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "HttpWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpWrapper::CHttpWrapper()
{
	m_strMessage	= "";
	m_common		= new CCommon();
	m_configuration	= new CConfiguration();
}

CHttpWrapper::~CHttpWrapper()
{
	delete m_configuration;
	delete m_common;
}

void CHttpWrapper::InitHttpWrapper()
{
	m_strMessage = "";
}

CString CHttpWrapper::GetLastError()
{
	return m_strMessage;
}

bool CHttpWrapper::SetLogin(CString UserID, CString Password) {
	InitHttpWrapper();

	W3Client w3Client;
	CString strResult;
	unsigned char buf[HTTP_RESULT_BUFFER_SIZE] = {0x00};

	if(w3Client.Connect(m_configuration->GetDomain(), m_configuration->GetPort())) {
		
		w3Client.AddPostArgument(HTTP_REQUEST_TYPE, 
			HTTP_OPERATION_OPERATION);

		w3Client.AddPostArgument(HTTP_REQUEST_ACTION, 
			HTTP_OPERATION_LOGIN);

		w3Client.AddPostArgument(HTTP_REQUEST_USERID, 
			m_common->URLEncode2(UserID));

		w3Client.AddPostArgument(HTTP_REQUEST_PASSWORD, 
			m_common->URLEncode2(Password));

		if(w3Client.Request(m_configuration->GetOperatorUri(), 
			W3Client::w3m::reqPost)) {
			w3Client.Response(buf, HTTP_RESULT_BUFFER_SIZE);
		}
		w3Client.Close();
		strResult.Format("%s", &buf);
	}
	return IsSuccess(strResult);
}

bool CHttpWrapper::SetLogout(CString UserID) {
	InitHttpWrapper();

	W3Client w3Client;
	CString strResult;
	unsigned char buf[HTTP_RESULT_BUFFER_SIZE] = {0x00};

	if(w3Client.Connect(m_configuration->GetDomain(), m_configuration->GetPort())) {
		
		w3Client.AddPostArgument(HTTP_REQUEST_TYPE, 
			HTTP_OPERATION_OPERATION);

		w3Client.AddPostArgument(HTTP_REQUEST_ACTION, 
			HTTP_OPERATION_LOGOUT);

		w3Client.AddPostArgument(HTTP_REQUEST_USERID, 
			m_common->URLEncode2(UserID));

		if(w3Client.Request(m_configuration->GetOperatorUri(), 
			W3Client::w3m::reqPost)) {
			w3Client.Response(buf, HTTP_RESULT_BUFFER_SIZE);
		}
		w3Client.Close();
		strResult.Format("%s", &buf);
	}
	return IsSuccess(strResult);
}

bool CHttpWrapper::SetSavePost(CString UserID, CString Subject, CString StaticData, CString Contents)
{
	InitHttpWrapper();

	W3Client w3Client;
	CString strResult;
	unsigned char buf[HTTP_RESULT_BUFFER_SIZE] = {0x00};

	if(w3Client.Connect(m_configuration->GetDomain(), m_configuration->GetPort())) {

		w3Client.AddPostArgument(HTTP_REQUEST_TYPE, 
			HTTP_OPERATION_OPERATION);

		w3Client.AddPostArgument(HTTP_REQUEST_ACTION, 
			HTTP_OPERATION_SAVEPOST);

		//w3Client.AddPostArgument(HTTP_REQUEST_USERID, 
		//	m_common->URLEncode2(UserID));

		w3Client.AddPostArgument(HTTP_REQUEST_SUBJECT, 
			m_common->URLEncode2(Subject));

		w3Client.AddPostArgument(HTTP_REQUEST_STATICDATA, 
			m_common->URLEncode2(StaticData));

		w3Client.AddPostArgument(HTTP_REQUEST_CONTENTS, 
			m_common->URLEncode2(Contents));

		if(w3Client.Request(m_configuration->GetOperatorUri(), 
			W3Client::w3m::reqPost)) {
			w3Client.Response(buf, HTTP_RESULT_BUFFER_SIZE);
		}
		w3Client.Close();
		strResult.Format("%s", &buf);
	}
	return IsSuccess(strResult);
}


bool CHttpWrapper::CheckSession() {
	InitHttpWrapper();

	W3Client w3Client;
	CString strResult;
	unsigned char buf[HTTP_RESULT_BUFFER_SIZE] = {0x00};

	if(w3Client.Connect(m_configuration->GetDomain(), m_configuration->GetPort())) {
		
		w3Client.AddPostArgument(HTTP_REQUEST_TYPE, 
			HTTP_OPERATION_OPERATION);

		w3Client.AddPostArgument(HTTP_REQUEST_ACTION, 
			HTTP_OPERATION_CHECKSESSION);

		if(w3Client.Request(m_configuration->GetOperatorUri(), 
			W3Client::w3m::reqPost)) {
			w3Client.Response(buf, HTTP_RESULT_BUFFER_SIZE);
		}
		w3Client.Close();
		strResult.Format("%s", &buf);
	}
	return IsSuccess(strResult);
}

bool CHttpWrapper::IsSuccess(CString Result)
{
	bool bResult = false;
	int iSplitPosition	= Result.FindOneOf("|");
	int iMessageLength	= Result.GetLength();

	if(iMessageLength > 0) {
		if(Result.Mid(0, iSplitPosition) == HTTP_RESULT_SUCCESS) {
			bResult = true;
		}
		m_strMessage = Result.Mid(iSplitPosition+1, 
			iMessageLength-iSplitPosition);
	}
	return bResult;
}
