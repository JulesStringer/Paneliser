#include "PartsSection.h"
#include "part.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CPartsSection::CPartsSection()
{
	m_pPart = NULL;
}
void CPartsSection::Write( const char* pszName, int nFile )
{
	std::string strName(pszName);
	strName += "\r\n";
	write(nFile, strName.c_str(), strName.length() );
	std::vector< CPart*>::iterator it;
	for( it = m_vecParts.begin(); it != m_vecParts.end(); it++ )
	{
        (*it)->Write( nFile );
	}
}
void CPartsSection::ProcessLine( const char* pszLine )
{
	if ( pszLine )
	{
		if ( strncmp( pszLine, "part:", 5 ) == 0 )
		{
			std::string strName = pszLine + 6;
			CPart* pPart = new CPart( strName.c_str() );
			m_mapParts[strName] = pPart;
			m_vecParts.push_back(pPart);
			m_pPart = pPart;
			//printf("Reading part : %s m_pPart= %x this = %x\n", strName.c_str(), m_pPart, this );
		}
		else if ( m_pPart )
		{
			//printf("m_pPart = %x this = %x\n", m_pPart, this);
			m_pPart->ProcessLine( pszLine );
			//printf("m_pPart = %x this = %x\n", m_pPart, this);
		}
	}
}
void CPartsSection::Flush()
{
}
CPart* CPartsSection::GetPart(const char* pszName)
{
	std::string strName(pszName);
	std::map< std::string, CPart*>::iterator it = m_mapParts.find( strName );
	if ( it != m_mapParts.end() )
	{
		return it->second;
	}
	return NULL;
}
void CPartsSection::Add(CPart* pPart)
{
	// renumber part
	std::string strType = pPart->Type();
	int nPart = 0;
	if ( strType.length() > 0 )
	{
		std::map< std::string, int >::iterator it = m_mapPartNos.find(strType);
		if ( it == m_mapPartNos.end() )
		{
			nPart = 1;
			m_mapPartNos[ strType ] = 1;
		}
		else
		{
			nPart = it->second + 1;
			it->second = nPart;
		}
		pPart->ReNumber( nPart );
		strType = pPart->ID();
		m_mapParts[strType] = pPart;
		m_vecParts.push_back(pPart);
	}
}
void CPartsSection::SetUniqueness()
{
    std::vector<CPart*>::iterator itP;
    for( itP = m_vecParts.begin(); itP != m_vecParts.end(); itP++ )
    {
        std::string strName = (*itP)->Name();
	    std::string strType = (*itP)->Type();
	    std::map< std::string, int >::iterator it = m_mapPartNos.find(strType);
        if ( it->second <= 1 )
        {
            (*itP)->SetUnique(1);
        }
        if ( strType.length() >= 3 )
        {
            // Could be a unique name
            std::vector<CPart*>::iterator it2;
            for( it2 = m_vecParts.begin(); it2 != m_vecParts.end(); it2++ )
            {
                if ( *it2 != *itP )
                {
                    if ( strName == (*it2)->Name() )
                    {
                        (*itP)->SetUnique(0);
                    }
                }
            }
        }
    }
}
void CPartsSection::Append(int nX, int nY, CPartsSection* pOther )
{
	std::map< std::string, CPart*>::iterator it;
	for( it = m_mapParts.begin(); it != m_mapParts.end(); it++ )
	{
		it->second->Offset(nX, nY);
		pOther->Add( it->second );
	}
}
std::string CPartsSection::ToString()
{
	std::string str = "Parts:-\n";
	std::map< std::string, CPart*>::iterator it;
	for( it = m_mapParts.begin(); it != m_mapParts.end(); it++ )
	{
		str += it->first;
		str += " ";
		str += it->second->ID();
		str += "\n";
	}
	return str;
}
