#include "connect.h"
#include "vertex.h"
#include "area.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CConnect::CConnect()
{
	m_nStartPin = 0;
	m_nEndPin = 0;
	m_pVertex = NULL;
}
int GetValue( char** psz )
{
	bool bSign = false;
	int nValue = 0;
	char* p = *psz;
    while(*p && !isdigit(*p) )
	{
		if ( *p == '-' ) bSign = true;
		p++;
	}
	while(isdigit(*p) )
	{
		nValue = 10 * nValue + *p - '0';
		p++;
	}
	if ( bSign ) nValue = -nValue;
	*psz = p;
	return nValue;
}
CConnect::CConnect(const char* pszRest)
{
/*
	connect: <index of this connection> <index of start pin> <index of end pin> <# segments> <flag if locked>
*/
    char* psz = (char*)pszRest;
    m_nSequence = GetValue( &psz );
	m_nStartPin = GetValue( &psz );
	m_nEndPin = GetValue( &psz );
	m_strRest = psz;
	m_pVertex = NULL;
}
CConnect::CConnect(const char* pszRest, int nSequence, int nStartPin, int nEndPin)
{
/*
	connect: <index of this connection> <index of start pin> <index of end pin> <# segments> <flag if locked>
*/
    m_nSequence = nSequence;
	m_strRest = pszRest;
	m_pVertex = NULL;
	m_nStartPin = nStartPin;
	m_nEndPin = nEndPin;
}
void CConnect::ProcessLine( const char* pszLine )
{
	/*
	ACTUAL
  connect: 1 1 0 2 0
    vtx: 1 15300000 74240000 12 0 0 0 0
    seg: 1 12 254000 0 0
	
	DOCUMENTED
	vtx: <first trace vertex: index> <x> <y> <first/last vertex, pad layer> <force via flag> <via pad width> <via hole dia> <tee id>
	seg: <first trace segment: index> <layer> <width> <via width> <via hole diameter>

	*/
	if ( strncmp( pszLine, "vtx:", 4) == 0 )
	{
		CVertex* pVertex = new CVertex( pszLine + 5);
		m_pVertex = pVertex;
		m_vecVertices.push_back( pVertex );
	}
	else if ( m_pVertex )
	{
		// deals with segments if present
		m_pVertex->ProcessLine( pszLine );
	}
}
const char* CConnect::Rest()
{
	return m_strRest.c_str();
}
int CConnect::StartPin() { return m_nStartPin; }
int CConnect::EndPin() { return m_nEndPin; }
void CConnect::Append(int nX, int nY, CConnect* pConnect)
{
	std::vector<CVertex*>::iterator it;
	for( it = m_vecVertices.begin(); it != m_vecVertices.end(); it++ )
	{
//		printf("original %d,%d \n", (*it)->X(), (*it)->Y());
		int X = nX + (*it)->X();
		int Y = nY + (*it)->Y();
		CVertex* pVertex = new CVertex(X, Y, (*it)->Index(), (*it)->Rest(), (*it)->Segment() );
		pConnect->Add(pVertex);
	}
}
void CConnect::Add(CVertex* pVertex)
{
	m_vecVertices.push_back(pVertex);
}
void CConnect::Write(int nFile)
{
	std::string strC = " connect: ";
	char sz[32];
	sprintf(sz,"%d %d %d ", m_nSequence, m_nStartPin, m_nEndPin);
	strC += sz;
	strC += m_strRest;
	strC += "\r\n";
	write(nFile, strC.c_str(), strC.length() );
	std::vector<CVertex*>::iterator it;
	for( it = m_vecVertices.begin(); it != m_vecVertices.end(); it++ )
	{
		(*it)->Write(nFile);
	}
}
void CConnect::SetSequence(int nSeq)
{
	m_nSequence = nSeq;
}
int CConnect::Sequence() { return m_nSequence; }
