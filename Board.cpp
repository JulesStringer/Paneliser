#include "Board.h"
#include "Section.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
//#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "FootprintSection.h"
#include "PartsSection.h"
#include "NetSection.h"
#include "TextSection.h"
#include "Process.h"

CBoard::CBoard()
{
    m_dX = 0;                 //    X  - Position of board on combined panel (MM)
	m_dY = 0;                 //    Y  - Position of board on combined panel (MM)
	m_dCX = 0;                //    CX - Width of board on combined panel(MM)
	m_dCY = 0;                //    CY - Height of board on combined panel(MM)
	m_pFootprintSection = NULL;
	m_pSection = NULL;
	m_bNoOutline = false;
}
CBoard::CBoard(const char* pszFile, double dWidth, double dHeight)
{
	m_strSourceFile = pszFile;
	m_dCX = dWidth;
	m_dCY = dHeight;
	m_dX = 0;
	m_dY = 0;
	m_pFootprintSection = NULL;
	m_pSection = NULL;
	m_bNoOutline = false;
}
CBoard::CBoard(const char* pszLine)
{
    m_dX = 0;                 //    X  - Position of board on combined panel (MM)
	m_dY = 0;                 //    Y  - Position of board on combined panel (MM)
	m_dCX = 0;                //    CX - Width of board on combined panel(MM)
	m_dCY = 0;                //    CY - Height of board on combined panel(MM)
	m_bNoOutline = false;
	int nField = 0;
	m_pFootprintSection = NULL;
	m_pSection = NULL;
	while( *pszLine == ' ')
	{
		pszLine++;
	};
	double dValue = 0;
	double dMultiplier = 10;
	double dFactor = 1;
	while( *pszLine )
	{
		if ( *pszLine == ' ' && nField < 5)
		{
			while(*pszLine == ' ')
			{
				pszLine++;
			}
			switch(nField)
			{
			case 0:
			    m_dX = dValue;
//				printf("m_dX = %f\n", m_dX);
				break;
			case 1:
			    m_dY = dValue;
//				printf("m_dY = %f\n", m_dY);
				break;
			case 2:
			    m_dCX = dValue;
//				printf("m_dCX = %f\n", m_dCX);
				break;
			case 3:
			    m_dCY = dValue;
//				printf("m_dCY = %f\n", m_dCY);
				break;
			}
			nField++;
			dValue = 0;
			dMultiplier = 10;
			dFactor = 1;
		}
		if ( isdigit(*pszLine) )
		{
			dValue = dMultiplier * dValue + (double)(*pszLine - '0') * dFactor;
			if ( dMultiplier < 5 )
			{
				dFactor *= 0.1;
			}
		}
		else if ( *pszLine == '.' )
		{
			dMultiplier = 1;
			dFactor = 0.1;
		}
		switch(nField)
		{
        case 4:
            m_strPrefix += *pszLine;
            break;
        case 5:
		    m_strSourceFile += *pszLine;
			break;
		}
		pszLine++;
	}
}
CBoard::~CBoard()
{
}
std::string CBoard::ToString()
{
	char sz[256];
	sprintf(sz,"Board: X=%f Y=%f CX=%f CY=%f Prefix: ", m_dX, m_dY, m_dCX, m_dCY);
	std::string str = sz;
	str += m_strPrefix;
	str += " Source : ";
	str += m_strSourceFile;
	return str;
}
void CBoard::Read()
{
	int nFile = open( m_strSourceFile.c_str(), O_RDONLY);
	if ( nFile == -1 )
	{
		printf("Unable to open %s\n", m_strSourceFile.c_str());
	}
	else
	{
		printf("Reading board : %s\n", m_strSourceFile.c_str());
		// Read whole file into memory
		off_t len = lseek(nFile, 0L, SEEK_END );
		lseek(nFile, 0L, SEEK_SET );
		char* psz = new char[len+1];
		char* pszFile = psz;
		read( nFile, psz, len);
		psz[len] = 0;
		close(nFile);
		std::string strLine;
		while( *psz )
		{
			if ( *psz == '\r' || *psz == '\n' )
			{
				ProcessLine( strLine.c_str() );
				strLine = "";
			}
			else
			{
				strLine += *psz;
			}
			psz++;
		}
		ProcessLine( strLine.c_str() );
		delete [] pszFile;
	}
}
void CBoard::ProcessLine( const char* pszLine )
{
	const char* psz = pszLine;
	if ( *pszLine == '[' )
	{
		std::string strName = "";
		while( *psz && *psz != ']' )
		{
			strName += *psz;
			psz++;
		}
		strName += ']';
		CSection* pSection = CSection::Create(strName.c_str());
		if ( pSection )
		{
			if ( m_pSection )
			{
				m_pSection->Flush();
			}
			m_pSection = pSection;
			m_mapSections[ strName ] = pSection;
			printf("Reading section %s\n", strName.c_str() );
			if ( strcmp( strName.c_str(), "[footprints]" ) == 0 )
			{
				m_pFootprintSection = (CFootprintSection*)pSection;
			}
		}
		else if ( m_pSection )
		{
			m_pSection->ProcessLine( pszLine );
		}
	}
	else if ( m_pSection )
	{
		m_pSection->ProcessLine( pszLine );
	}
}
CSection* CBoard::GetSection(const char* pszName)
{
	std::string strName(pszName);
	std::map<std::string, CSection*>::iterator it = m_mapSections.find(strName);
	if ( it == m_mapSections.end() )
	{
		return NULL;
	}
	return it->second;
}
void CBoard::AddSection( const char* pszName, CSection* pSection)
{
	std::string strName(pszName);
	m_mapSections[strName] = pSection;
	if ( strcmp(pszName, "[footprints]") == 0 )
	{
		m_pFootprintSection = (CFootprintSection*)pSection;
	}
}
void CBoard::WriteSection( const char* pszName, int nFile )
{
	std::string strName = pszName;
	std::map<std::string, CSection*>::iterator it = m_mapSections.find(strName);
	if ( it != m_mapSections.end() )
	{
		it->second->Write( pszName, nFile );
	}
}
void CBoard::SetNoOutline(bool bNoOutline)
{
	m_bNoOutline = bNoOutline;
}
void CBoard::Write()
{
	int nFile = open( m_strSourceFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                        S_IRWXU );
    if ( nFile == -1 )
	{
		if ( errno == EACCES )
		{
			printf("Access denied creating %s\n", m_strSourceFile.c_str());
		}
		else
		{
			printf("Error %d Unable to create %s\n", errno, m_strSourceFile.c_str());
		}
	}
	else
	{
		WriteSection("[options]", nFile);
		WriteSection("[footprints]", nFile);
		WriteSection("[board]", nFile );
		WriteSection("[parts]", nFile );
		WriteSection("[nets]", nFile);
		WriteSection("[texts]", nFile);
		WriteSection("[end]", nFile);
		close(nFile);
	}
}
bool CBoard::IsValid()
{
	if ( m_strSourceFile.length() == 0 )
	{
		return false;
	}
	if ( m_strPrefix.length() == 0 )
	{
		return false;
	}
	if ( m_dCX <= 0 || m_dCY <= 0 )
	{
		return false;
	}
	return true;
}
void CBoard::MergeFootprints( CFootprintSection* pFootprints )
{
	if ( m_pFootprintSection )
	{
		m_pFootprintSection->Merge(pFootprints);
	}
}
void CBoard::WriteOutline( int nFile, int nSeq )
{
	if ( !m_bNoOutline )
	{
		char sz[256];
		sprintf(sz,"outline: 4 %d\r\n", nSeq );
		write(nFile, sz, strlen(sz));

		sprintf(sz,"  corner: 1 %d %d 0\r\n", (int)(m_dX * 1000000), (int)(m_dY * 1000000));
		write(nFile, sz, strlen(sz));

		sprintf(sz,"  corner: 2 %d %d 0\r\n", (int)(m_dX * 1000000), (int)((m_dY + m_dCY) * 1000000));
		write(nFile, sz, strlen(sz));

		sprintf(sz,"  corner: 3 %d %d 0\r\n", (int)((m_dX + m_dCX) * 1000000), (int)((m_dY + m_dCY) * 1000000));
		write(nFile, sz, strlen(sz));

		sprintf(sz,"  corner: 4 %d %d 0\r\n", (int)((m_dX + m_dCX) * 1000000), (int)(m_dY * 1000000));
		write(nFile, sz, strlen(sz));

		sprintf(sz,"\r\n");
		write(nFile, sz, strlen(sz));
	}
}
void CBoard::AppendParts( CPartsSection* pPartsSection )
{
	CPartsSection* pParts = (CPartsSection*)GetSection("[parts]");
	if ( pParts )
	{
		pParts->Append( (int)(m_dX * 1000000), (int)(m_dY * 1000000), pPartsSection );
	}
}
void CBoard::AppendNets( CNetsSection* pNetsSection, CProcess* pProcess )
{
	printf("Appending nets for %s\n", m_strPrefix.c_str());
	CPartsSection* pParts = (CPartsSection*)GetSection("[parts]");
	CNetsSection* pNets = (CNetsSection*)GetSection("[nets]");
	if ( pParts && pNets)
	{
		printf("Got parts and nets\n");
		pNets->Append( (int)(m_dX * 1000000), (int)(m_dY * 1000000), m_strPrefix.c_str(), pNetsSection, pParts, pProcess );
	}
}
void CBoard::AppendTexts( CTextSection* pTextSection )
{
	CTextSection* pTexts = (CTextSection*)GetSection("[texts]");
	if ( pTexts )
	{
		pTexts->Append( (int)(m_dX * 1000000), (int)(m_dY * 1000000), pTextSection );
	}
}
