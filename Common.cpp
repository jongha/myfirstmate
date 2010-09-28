// Common.cpp: implementation of the CCommon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFirstMateBar.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommon::CCommon()
{
	
}

CCommon::~CCommon()
{
	
}

CString CCommon::URLEncode1(CString sIn)
{
    CString sOut;
	
    int k;
    const int nLen = sIn.GetLength() + 1;
	
    register LPBYTE pOutTmp = NULL;
    LPBYTE pOutBuf = NULL;
    register LPBYTE pInTmp = NULL;
    LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
    BYTE b = 0;
	
    k = 0;
	
    pInTmp = pInBuf;
    while(*pInTmp) {
        if (!isalnum(*pInTmp) && !isalnum(*pInTmp))
            k++;
        pInTmp++;
    }
	
    pOutBuf = (LPBYTE)sOut.GetBuffer(nLen  + 2 * k); //new BYTE [nLen  + 3 * k];
	
    if(pOutBuf) {
        pInTmp	= pInBuf;
		pOutTmp = pOutBuf;
		
		while (*pInTmp) {
			if(isalnum(*pInTmp))
                *pOutTmp++ = *pInTmp;
			else
				if(isspace(*pInTmp))
					*pOutTmp++ = '+';
				else {
					*pOutTmp++ = '%';
					*pOutTmp++ = ByteToHex(*pInTmp>>4);
					*pOutTmp++ = ByteToHex(*pInTmp%16);
				}
				pInTmp++;
		}
		
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
    }
    sIn.ReleaseBuffer();
    return sOut;
}

CString CCommon::URLEncode2(CString sIn)
{
    CString sOut;
	
    const int nLen = sIn.GetLength() + 1;
	
    register LPBYTE pOutTmp = NULL;
    LPBYTE pOutBuf = NULL;
    register LPBYTE pInTmp = NULL;
    LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
    BYTE b = 0;
	
    //alloc out buffer
    pOutBuf = (LPBYTE)sOut.GetBuffer(nLen  * 3 - 2);//new BYTE [nLen  * 3];
	
    if(pOutBuf) {
        pInTmp	= pInBuf;
		pOutTmp = pOutBuf;
		
		while (*pInTmp) {
			if(isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else
				if(isspace(*pInTmp))
					*pOutTmp++ = '+';
				else {
					*pOutTmp++ = '%';
					*pOutTmp++ = ByteToHex(*pInTmp>>4);
					*pOutTmp++ = ByteToHex(*pInTmp%16);
				}
				pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
    }
    sIn.ReleaseBuffer();
    return sOut;
}
