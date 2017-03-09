#include "part.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CPart::CPart()
{
	m_nNumber = 0;
	m_nX = 0;
	m_nY = 0;
        m_bUnique = 0;
}
CPart::CPart(const char* pszName)
{
	m_nNumber = 0;
	m_nX = 0;
	m_nY = 0;
	m_strType = "";
        m_bUnique = 0;
        m_strName = pszName;
	while( *pszName && !isdigit(*pszName) )
	{
		m_strType += *pszName;
		pszName++;
	}
	if ( m_strType.length() >= 3)
    {
        m_bUnique = 2;
    }
	while( isdigit( *pszName ) )
	{
		m_nNumber = 10 * m_nNumber + *pszName - '0';
		pszName++;
	}
}
void CPart::Write( int nFile )
{
	std::string strName = "part: ";
	strName += ID();
	strName += "\r\n";
	write( nFile, strName.c_str(), strName.length());
	write( nFile, m_strText.c_str(), m_strText.length());
	char sz[256];
	sprintf(sz,"  pos: %d %d", m_nX, m_nY );
	std::string strPos = sz;
	strPos += m_strPosRest;
	strPos += "\r\n";
	write( nFile, strPos.c_str(), strPos.length() );
	std::string strCRLF = "\r\n";
	write( nFile, strCRLF.c_str(), strCRLF.length() );
}
void CPart::ProcessLine( const char* pszLine )
{
	if ( pszLine )
	{
		const char* psz  = pszLine;
		while( *psz == ' ' )
		{
			psz++;
		}
		if ( strncmp( psz, "pos:", 4 ) == 0 )
		{
			psz += 5;
			while(isdigit(*psz) )
			{
				m_nX = 10 * m_nX + *psz - '0';
				psz++;
			}
			while( *psz == ' ')
			{
				psz++;
			}
			while(isdigit(*psz) )
			{
				m_nY = 10 * m_nY + *psz - '0';
				psz++;
			}
			m_strPosRest = psz;
		}
		else
		{
			try
			{
				if ( m_strText.length() > 0 )
				{
					m_strText += "\r\n";
				}
				m_strText += pszLine;
			}
			catch(...)
			{
				printf("Exception adding %s\n", pszLine );
			}
		}
	}
}
const char* CPart::Type()
{
	return m_strType.c_str();
}
void CPart::ReNumber(int nNo)
{
	m_nNumber = nNo;
}
std::string CPart::ID()
{
        if( m_bUnique )
        {
                return m_strName;
        }
        else
        {
     		std::string strID = m_strType;
		char sz[32];
		sprintf(sz, "%d", m_nNumber );
		strID += sz;
		return strID;
        }
}
void CPart::Offset( int nX, int nY)
{
	m_nX += nX;
	m_nY += nY;
}
void CPart::SetUnique(int nUnique)
{
    m_bUnique = nUnique;
}
std::string CPart::Name()
{
    return m_strName;
}
