#include "corner.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CCorner::CCorner()
{
	m_nIndex = 0;
	m_nX = 0;
	m_nY = 0;
}
CCorner::CCorner(const char* psz)
{
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
	//printf("Corner %d %d\n", m_nX, m_nY);
}
CCorner::CCorner(int X, int Y, int nIndex, const char* pszRest)
{
	m_nX = X;
	m_nY = Y;
	m_nIndex = nIndex;
	m_strRest = pszRest;
}
int CCorner::X() { return m_nX; }
int CCorner::Y() { return m_nY; }
const char* CCorner::Rest(){ return m_strRest.c_str(); }
void CCorner::Write(int nFile)
{
	char sz[256];
	sprintf(sz,"   corner: %d %d %d %s\r\n", m_nIndex, m_nX, m_nY, m_strRest.c_str());
	write( nFile, sz, strlen(sz));
}
