#include "FootprintSection.h"
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>


CFootprintSection::CFootprintSection()
{
}
void CFootprintSection::Merge(CFootprintSection* pOther)
{
	std::map<std::string, std::string>::iterator it;
	for( it = m_mapFootprints.begin(); it != m_mapFootprints.end(); it++ )
	{
		pOther->Add( it->first.c_str(), it->second.c_str());
	}
}
void CFootprintSection::Write( const char* pszName, int nFile )
{
	printf("Writing Footprints\n");
	std::string strName = pszName;
	strName += "\r\n";
	write( nFile, strName.c_str(), strName.length());
	std::map<std::string, std::string>::iterator it;
	for( it = m_mapFootprints.begin(); it != m_mapFootprints.end(); it++ )
	{
		std::string strF = "name:";
		strF += it->first;
		strF += "\r\n";
		write( nFile, strF.c_str(), strF.length());
		
		write( nFile, it->second.c_str(), it->second.length());
	}
}
void CFootprintSection::ProcessLine( const char* pszLine )
{
//	printf("%s\n", pszLine);
	if ( strncmp( pszLine, "name:", 5 ) == 0 )
	{
		Flush();
        m_strCurrentName = pszLine + 6;
		m_strCurrentPrint = "";
		printf("Current Name %s\n", m_strCurrentName.c_str());
	}
	else
	{
		m_strCurrentPrint += pszLine;
		m_strCurrentPrint += "\r\n";
	}
}
void CFootprintSection::Add( const char* pszName, const char* pszContent )
{
	std::string strName = pszName;
	std::string strContent = pszContent;
	m_mapFootprints[strName] = strContent;
}
void CFootprintSection::Flush()
{
	if ( m_strCurrentName.length() > 0 )
	{
		Add( m_strCurrentName.c_str(), m_strCurrentPrint.c_str() );
	}
	m_strCurrentName = "";
	m_strCurrentPrint = "";
}
