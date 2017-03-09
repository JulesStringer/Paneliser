#include "vertex.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CVertex::CVertex()
{
	m_nIndex = 0;
	m_nX = 0;
	m_nY = 0;
}
CVertex::CVertex( const char* psz)
{
	//printf("%s\n", psz);
	m_nIndex = 0;
	m_nX = 0;
	m_nY = 0;
	while( *psz == ' ' )
	{
		psz++;
	}
	while( isdigit(*psz) )
	{
		m_nIndex = 10 * m_nIndex + *psz - '0';
		psz++;
	}
	while( *psz == ' ' )
	{
		psz++;
	}
	while( isdigit(*psz) )
	{
		m_nX = 10 * m_nX + *psz - '0';
		psz++;
	}
	while( *psz == ' ' )
	{
		psz++;
	}
	while( isdigit(*psz) )
	{
		m_nY = 10 * m_nY + *psz - '0';
		psz++;
	}
	m_strRest = psz;
	//printf("Vertex %d %d\n", m_nX, m_nY);
}
CVertex::CVertex(int nX, int nY, int nIndex, const char* pszRest, const char* pszSegment )
{
	m_nX = nX;
	m_nY = nY;
	m_nIndex = nIndex;
	m_strRest = pszRest;
	m_strSegment = pszSegment;
	//printf("Vertex %d %d\n", m_nX, m_nY);
}
void CVertex::ProcessLine( const char* psz)
{
	if ( strncmp( psz, "seg:", 4) == 0 )
	{
		m_strSegment = psz;
	}
}
void CVertex::Write(int nFile)
{
    char sz[256];
	sprintf(sz,"  vtx: %d %d %d %s\r\n", m_nIndex, m_nX, m_nY, m_strRest.c_str());
	write(nFile, sz, strlen(sz));
	if ( m_strSegment.length() > 0 )
	{
		std::string str = "  ";
		str += m_strSegment;
		str += "\r\n";
		write(nFile, str.c_str(), str.length());
	}
}
int CVertex::X(){ return m_nX; }
int CVertex::Y(){ return m_nY; }
int CVertex::Index(){ return m_nIndex; }
const char* CVertex::Rest(){ return m_strRest.c_str(); }
const char* CVertex::Segment(){ return m_strSegment.c_str(); }
 
