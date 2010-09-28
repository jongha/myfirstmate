// HtmlParser.h: interface for the CHtmlParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLPARSER_H__9DFF6895_20B8_436B_80AC_3CA461499E83__INCLUDED_)
#define AFX_HTMLPARSER_H__9DFF6895_20B8_436B_80AC_3CA461499E83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PARSER_SIZE	256
#define MFM_TAG_NAME			"${DATA%d}"
#define MFM_TAG_NAME_SPLITER	"|"
#define MFM_TAG_STATIC_DELIM	"¡¹"

class CHtmlParser  
{
public:
	BOOL GetParseOfContents(BOOL ParseTag=TRUE);
	CString GetAbsoluteLocation(CString Location);
	CHtmlParser(CString Location, CString HtmlContents);
	virtual ~CHtmlParser();

	int PostDataCount;
	CString PostStaticData;
	CString PostContents;

private:
	CString GetAbsoluteLocation(CString Location, CString Src);
	BOOL InitParseContents();
	CString m_Location;
	CString m_HtmlContents;

	int m_iMatchLength;
	int* m_iMatchPosition;
	int* m_iMatchStrLen;
	CString* m_strStaticData;
	CString* m_strParseContents;
};

#endif // !defined(AFX_HTMLPARSER_H__9DFF6895_20B8_436B_80AC_3CA461499E83__INCLUDED_)
