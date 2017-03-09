#include "Text.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CText::CText()
{
	m_nX = 0;
	m_nY = 0;
}
CText::CText(const char* pszLine)
{
	m_nX = 0;
	m_nY = 0;
	while(*pszLine == ' ')
	{
		pszLine++;
	}
	if ( *pszLine == '\"' )
	{
		pszLine++;
		while( *pszLine && *pszLine != '\"' )
		{
			m_strText += *pszLine;
			pszLine++;
		}
		if ( *pszLine == '\"' )
		{
			pszLine++;
		}
		while( *pszLine == ' ' )
		{
			pszLine++;
		}
		bool bSign = *pszLine == '-' ? true : false;
		if ( bSign )
        {
            pszLine++;
        }
		while( isdigit(*pszLine) )
		{
			m_nX = 10 * m_nX + *pszLine - '0';
			pszLine++;
		}
		if ( bSign )
        {
            m_nX = -m_nX;
        }
		while( *pszLine == ' ' )
		{
			pszLine++;
		}
		bSign = *pszLine == '-' ? true : false;
		if ( bSign )
        {
            pszLine++;
        }
		while( isdigit(*pszLine) )
		{
			m_nY = 10 * m_nY + *pszLine - '0';
			pszLine++;
		}
		if ( bSign )
        {
            m_nY = -m_nY;
        }
		m_strRest = pszLine;
	}
}
CText::CText(int nX, int nY, const char* pszText, const char* pszRest )
{
	m_nX = nX;
	m_nY = nY;
	m_strText = pszText;
	m_strRest = pszRest;
}
void CText::Write(int nFile)
{
	std::string str = "  text: \"";
	str += m_strText;
	str += "\"";
	char sz[256];
	sprintf(sz, " %d %d", m_nX, m_nY );
	str += sz;
	str += m_strRest;
	str += "\r\n";
	write(nFile, str.c_str(), str.length());
}
int CText::X(){ return m_nX; }
int CText::Y(){ return m_nY; }
const char* CText::Text(){ return m_strText.c_str(); }
const char* CText::Rest(){ return m_strRest.c_str(); }
bool CText::Valid()
{
	if ( m_nX >= 0 && m_nY >= 0 && m_strText.length()> 0 && m_strRest.length() > 0 )
	{
		return true;
	}
	return false;
}
