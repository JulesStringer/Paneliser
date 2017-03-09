#include "NetSection.h"
#include "Net.h"
#include "PartsSection.h"
#include "Process.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>


CNetsSection::CNetsSection()
{
	m_pNet = NULL;
}
void CNetsSection::Write( const char* pszName, int nFile )
{
	std::string strName(pszName);
	strName += "\r\n";
	write(nFile, strName.c_str(), strName.length() );	
	std::vector<CNet*>::iterator it;
	for(it = m_vecNets.begin(); it != m_vecNets.end(); it++ )
	{
		(*it)->Write(nFile);
	}
}
void CNetsSection::ProcessLine( const char* pszLine )
{
	if ( pszLine )
	{
		if ( strncmp( pszLine, "net:", 4) == 0 )
		{
			CNet* pNet = new CNet( pszLine + 5);
			m_pNet = pNet;
			m_vecNets.push_back(pNet);
			//printf("Processing net %s\n", pNet->Name());
		}
		else if ( m_pNet )
		{
			m_pNet->ProcessLine( pszLine );
		}
	}
}
void CNetsSection::Flush()
{
}
void CNetsSection::Append( int nX, int nY, const char* pszPrefix, CNetsSection* pNetSection, CPartsSection* pPartsSection, CProcess* pProcess )
{
//	printf("%s", pPartsSection->ToString().c_str());
	std::vector<CNet*>::iterator it;
	int nIndex = 1;
	for( it = m_vecNets.begin(); it != m_vecNets.end(); it++ )
	{
		if ( (*it)->HasContent() )
		{
			(*it)->Append(nX, nY, pszPrefix, pNetSection, pPartsSection, pProcess );
			nIndex++;
		}
	}
}
CNet* CNetsSection::GetNet(const char* pszNet)
{
	std::vector<CNet*>::iterator it;
	for( it = m_vecNets.begin(); it != m_vecNets.end(); it++ )
	{
		if ( strcmp((*it)->Name(), pszNet) == 0 )
		{
			return *it;
		}
	}
	return NULL;
}
void CNetsSection::Add(CNet* pNet)
{
	m_vecNets.push_back(pNet);
}
