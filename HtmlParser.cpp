// HtmlParser.cpp: implementation of the CHtmlParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myfirstmatebar.h"
#include "HtmlParser.h"
#include "RexAlgorithm.h"
#include "RexInterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlParser::CHtmlParser(CString Location, CString HtmlContents)
{
	m_iMatchLength	= 0;
	m_Location		= GetAbsoluteLocation(Location);
	m_HtmlContents	= HtmlContents;

	PostDataCount	= 0;
	PostStaticData	= "";
	PostContents	= "";

	m_iMatchPosition	= new int[MAX_PARSER_SIZE];
	m_iMatchStrLen		= new int[MAX_PARSER_SIZE];
	m_strStaticData		= new CString[MAX_PARSER_SIZE];
	m_strParseContents	= new CString[MAX_PARSER_SIZE*2+1];
}

CHtmlParser::~CHtmlParser()
{
	delete[] m_strParseContents;
	delete[] m_strStaticData;
	delete[] m_iMatchStrLen;
	delete[] m_iMatchPosition;
}

CString CHtmlParser::GetAbsoluteLocation(CString Location)
{
	return Location.Mid(0, Location.ReverseFind('/')+1);
}

BOOL CHtmlParser::GetParseOfContents(BOOL ParseTag)
{
	InitParseContents();
	PostDataCount = m_iMatchLength;

	CString strParseTag = "";
	int iStartPosition = 0;
	for(int i=0; i<m_iMatchLength; i++) {
		strParseTag.Format(MFM_TAG_NAME, i+1);
		PostStaticData	+= strParseTag + MFM_TAG_NAME_SPLITER + m_strStaticData[i] + MFM_TAG_STATIC_DELIM;
		PostContents	+= m_HtmlContents.Mid(iStartPosition, m_iMatchPosition[i] - iStartPosition);

		if(ParseTag == TRUE) { PostContents	+= strParseTag;
		}else { PostContents += m_strStaticData[i]; }

		iStartPosition	= m_iMatchPosition[i] + m_iMatchStrLen[i];
	}
	PostContents += m_HtmlContents.Mid(iStartPosition, m_HtmlContents.GetLength() - iStartPosition);
    return TRUE;
}

BOOL CHtmlParser::InitParseContents()
{
	m_iMatchLength = 0;
	CString strContent(m_HtmlContents);
	REXI_Search rexs; 
	if(rexs.SetRegexp("src[=]+['\"]+([^'\"]*)").eErrCode == REXI_DefErr::eNoErr) {
		const char*	pCur = (LPCTSTR)m_HtmlContents;
		int			nMatchLen;
		bool		bEosFound= true;
		while(rexs.Find(pCur, nMatchLen, bEosFound)){
			m_iMatchPosition[m_iMatchLength]	= (pCur-(LPCTSTR)m_HtmlContents)-nMatchLen + 5;
			m_iMatchStrLen[m_iMatchLength]		= nMatchLen - 5;
			m_strStaticData[m_iMatchLength]		= 
				GetAbsoluteLocation(m_Location, 
					m_HtmlContents.Mid(m_iMatchPosition[m_iMatchLength], m_iMatchStrLen[m_iMatchLength])
				);
			m_iMatchLength++;
		}
	}
	return TRUE;
}

CString CHtmlParser::GetAbsoluteLocation(CString Location, CString Src)
{
	Location = GetAbsoluteLocation(Location);
	Src.MakeLower();

	CString strAbsoluteLocation = "";
	int iLocationLength = Location.GetLength();
	int iSrcLength		= Src.GetLength();

	if(iSrcLength > 7 &&
		Src.Left(7) == "http://") {
		strAbsoluteLocation = Src;

	}else if(iSrcLength > 3 &&
		Src.Left(3) == "../") {
		int iPosition = Location.Mid(0, iLocationLength-1).ReverseFind('/');
		Location = Location.Mid(0, iPosition+1);
		strAbsoluteLocation = Location + Src.Mid(3, iSrcLength-3);

	}else if(iSrcLength > 2 &&
		Src.Left(2) == "./") {
		strAbsoluteLocation = Location + Src.Mid(2, iSrcLength-2);

	}else if(iSrcLength > 1 &&
		Src.Left(1) == "/") {
		int iPosition = Location.Find('/', 8);
		Location = Location.Mid(0, iPosition+1);
		strAbsoluteLocation = Location + Src.Mid(1, iSrcLength-1);

	}else if(iSrcLength > 1 &&
		Src.Left(1) != "/") {
		strAbsoluteLocation = Location + Src;
	}
	return strAbsoluteLocation;
}
