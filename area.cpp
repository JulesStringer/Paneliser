#include "area.h"
#include "corner.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CArea::CArea()
{
	m_nSequence = 0;
}
CArea::CArea(const char* psz)
{
/*
area: <index of this copper area> <# corners> <layer> <hatch style>
*/
    m_nSequence = 0;
    while( *psz && !isdigit(*psz) )
	{
		psz++;
	}
	while( isdigit(*psz) )
	{
		m_nSequence = 10 * m_nSequence + *psz - '0';
		psz++;
	}
	m_strArea = psz;
}
CArea::CArea(const char* psz, int nSequence)
{
/*
area: <index of this copper area> <# corners> <layer> <hatch style>
*/
    m_nSequence = nSequence;
	m_strArea = psz;
}
void CArea::ProcessLine( const char* psz)
{
	if ( strncmp( psz, "corner:",7) == 0 )
	{
		/*
		corner: <index of first corner> <x> <y> <style of next side> <flag if last corner of this contour>
		*/
		CCorner* pCorner = new CCorner( psz + 8 );
		m_vecCorner.push_back( pCorner );
	}
}
const char* CArea::Area()
{
	return m_strArea.c_str();
}
void CArea::Append(int nX, int nY, CArea* pArea)
{
	int nIndex = 1;
	std::vector<CCorner*>::iterator it;
	for( it = m_vecCorner.begin(); it != m_vecCorner.end(); it++, nIndex++ )
	{
		int X = nX + (*it)->X();
		int Y = nY + (*it)->Y();
		CCorner* pCorner = new CCorner(X, Y, nIndex, (*it)->Rest());
		pArea->Add(pCorner);
	}
}
void CArea::Add(CCorner* pCorner)
{
	m_vecCorner.push_back(pCorner);
}
void CArea::Write(int nFile)
{
	std::string str = "  area: ";
	char sz[32];
	sprintf(sz, "%d ", m_nSequence );
	str += sz;
	str += m_strArea;
	str += "\r\n";
	write( nFile, str.c_str(), str.length() );
	std::vector<CCorner*>::iterator it;
	for( it = m_vecCorner.begin(); it != m_vecCorner.end(); it++)
	{
		(*it)->Write(nFile);
	}
}
void CArea::SetSequence(int nSequence)
{
	m_nSequence = nSequence;
}
int CArea::Sequence()
{
	return m_nSequence;
}
