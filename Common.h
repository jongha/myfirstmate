// Common.h: interface for the CCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMON_H__A97ED487_E378_40B7_8363_F25FDCF65F8A__INCLUDED_)
#define AFX_COMMON_H__A97ED487_E378_40B7_8363_F25FDCF65F8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCommon  
{
public:
	CCommon();
	virtual ~CCommon();
	CString URLEncode1(CString sIn);
	CString URLEncode2(CString sIn);
	inline BYTE ByteToHex(const BYTE &x) { return x > 9 ? x + 55: x + 48; }
};

#endif // !defined(AFX_COMMON_H__A97ED487_E378_40B7_8363_F25FDCF65F8A__INCLUDED_)
