#include "optionssection.h"
#include <strings.h>

COptionsSection::COptionsSection()
{
}
COptionsSection::COptionsSection( COptionsSection* pOptionsSection) :
    CSection(pOptionsSection)
{
}
COptionsSection::~COptionsSection()
{
}
void COptionsSection::Replace( const char* pszOption, const char* pszValue)
{
	// look for line beginning with pszOption:
	std::string strTarget = pszOption;
	strTarget += ":";
	std::vector<std::string>::iterator it;
	for( it = m_vecLines.begin(); it != m_vecLines.end(); it++ )
	{
		if ( strncmp( (*it).c_str(), strTarget.c_str(), strTarget.length() ) == 0 )
		{
			strTarget += pszValue;
			*it = strTarget;
			break;
		}
	}
}
