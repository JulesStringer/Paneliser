#include "Section.h"
#include "OptionsSection.h"
#include "FootprintSection.h"
#include "BoardSection.h"
#include "PartsSection.h"
#include "NetSection.h"
#include "TextSection.h"

#include <strings.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
 

CSection::CSection()
{
}
CSection::CSection(CSection* pFrom)
{
	std::vector<std::string>::iterator it;
	for( it = pFrom->m_vecLines.begin(); it != pFrom->m_vecLines.end(); it++ )
	{
		std::string str = *it;
		m_vecLines.push_back(str);
	}
}
CSection* CSection::Create( const char* pszName )
{
    CSection* pSection = NULL;
	if ( strcmp(pszName,"[options]") == 0 )
	{
		pSection = new COptionsSection();
	}
	else if ( strcmp(pszName,"[footprints]") == 0 )
	{
		pSection = new CFootprintSection();
	}
	else if ( strcmp( pszName,"[board]") == 0 )
	{
		pSection = new CBoardSection();
	}
	else if ( strcmp( pszName, "[solder_mask_cutouts]") == 0 )
	{
		pSection = new CSection();
	}
	else if ( strcmp( pszName, "[parts]") == 0 )
	{
		pSection = new CPartsSection();
	}
	else if ( strcmp( pszName, "[nets]") == 0 )
	{
		pSection = new CNetsSection();
	}
	else if ( strcmp( pszName, "[texts]") == 0 )
	{
		pSection = new CTextSection();
	}
	else if ( strcmp( pszName, "[end]") == 0 )
	{
		pSection = new CSection();
	}
	else
	{
		printf("Unrecognised section [%s]\n", pszName);
	}
	return pSection;
}
void CSection::ProcessLine( const char* pszLine )
{
	std::string str = pszLine;
	m_vecLines.push_back(str);
}
void CSection::Write( const char* pszName, int nFile )
{
	std::string strName = pszName;
	strName += "\r\n";
	write( nFile, strName.c_str(), strName.length() );
	std::vector<std::string>::iterator it;
	for( it = m_vecLines.begin(); it != m_vecLines.end(); it++ )
	{
		std::string strLine = *it;
		strLine += "\r\n";
		write(nFile, strLine.c_str(), strLine.length() );
	}
}
void CSection::Flush()
{
}