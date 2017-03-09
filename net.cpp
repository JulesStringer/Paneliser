#include "Net.h"
#include "Pin.h"
#include "Connect.h"
#include "Area.h"
#include "part.h"
#include "PartsSection.h"
#include "NetSection.h"
#include "Process.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CNet::CNet()
{
	m_pConnect = NULL;
	m_pArea = NULL;
}
/*
net: "RS485A" 3 2 0 0 0 0 1
net: <name> <npins> <nconnect> <narea> 0 0 0 1
*/
CNet::CNet( const char* pszLine )
{
	m_pConnect = NULL;
	m_pArea = NULL;
	if ( *pszLine == '\"' )
	{
		pszLine++;
		while( *pszLine && *pszLine != '\"' )
		{
			m_strName += *pszLine;
			pszLine++;
		}
		pszLine++;
		// skip pin, connect and area counts
		while(*pszLine && !isdigit(*pszLine) )pszLine++;
		while(*pszLine && isdigit(*pszLine) )pszLine++;
		while(*pszLine && !isdigit(*pszLine) )pszLine++;
		while(*pszLine && isdigit(*pszLine) )pszLine++;
		while(*pszLine && !isdigit(*pszLine) )pszLine++;
		while(*pszLine && isdigit(*pszLine) )pszLine++;
		m_strRest = pszLine;
	}
}
CNet::CNet( const char* pszName, const char* pszRest)
{
	m_pConnect = NULL;
	m_pArea = NULL;
	m_strName = pszName;
	m_strRest = pszRest;
}
/*
  pin: 1 RJ45.7
  pin: 2 U3.6
  pin: 3 RJ45.2
  connect: 1 1 0 2 0
    vtx: 1 15300000 74240000 12 0 0 0 0
    seg: 1 12 254000 0 0
    vtx: 2 13690000 74270000 0 0 0 0 0
    seg: 2 12 254000 0 0
    vtx: 3 11500000 77190000 11 0 0 0 0
*/
/*
[nets]
net: <name of net> <# pins> <# connections> <# areas> <default trace width> <default via pad width> <default via hole dia> <visibility>
pin: <index of first pin in net> <name, such "U1.5">
pin: <index of next pin in net> <name, such "U1.5">
...
pin: <index of last pin in net> <name, such "U1.5">
connect: <index of this connection> <index of start pin> <index of end pin> <# segments> <flag if locked>
vtx: <first trace vertex: index> <x> <y> <first/last vertex, pad layer> <force via flag> <via pad width> <via hole dia> <tee id>
seg: <first trace segment: index> <layer> <width> <via width> <via hole diameter>
vtx: <next trace vertex: index> <x> <y> <first/last vertex, pad layer> <force via flag> <via pad width> <via hole dia> <tee id>
seg: <next trace segment: index> <layer> <width> <via width> <via hole diameter>
...
seg: <last trace segment: index> <layer> <width> <via width> <via hole diameter>
vtx: <last trace vertex: index> <x> <y> <first/last vertex, pad layer> <force via flag> <via pad width> <via hole dia> <tee id>
area: <index of this copper area> <# corners> <layer> <hatch style>
corner: <index of first corner> <x> <y> <style of next side> <flag if last corner of this contour>
corner: <index of next corner> <x> <y> <style of next side> <flag if last corner of this contour>
...
corner: <index of last corner> <x> <y> <style of next side> <flag if last corner of this contour>
*/
void CNet::ProcessLine( const char* pszLine )
{
	if ( pszLine )
	{
		const char* psz = pszLine;
		while( *psz == ' ' )
		{
			psz++;
		}
		if ( strncmp( psz, "pin:", 4) == 0 )
		{
			CPin* pPin = new CPin(psz + 5);
			m_vecPins.push_back(pPin);
		}
		else if ( strncmp( psz, "connect:", 8) == 0 )
		{
			CConnect* pConnect = new CConnect( psz + 9);
			m_vecConnect.push_back(pConnect);
			m_pConnect = pConnect;
			m_pArea = NULL;
		}
		else if ( strncmp( psz, "area:", 5) == 0 )
		{
			m_pConnect = NULL;
			CArea* pArea = new CArea( psz + 6);
			m_pArea = pArea;
			m_vecAreas.push_back( pArea );
		}
		else if ( m_pConnect )
		{
			m_pConnect->ProcessLine( psz );
		}
		else if ( m_pArea )
		{
			m_pArea->ProcessLine(psz);
		}
	}
}
const char* CNet::Name()
{
	return m_strName.c_str();
}
void CNet::Append(int nX, int nY, const char* pszPrefix, CNetsSection* pNetSection, CPartsSection* pPartsSection, CProcess* pProcess)
{
	//net: "RS485A" 3 2 0 0 0 0 1
	std::string strName = "";
	if ( !pProcess->MergeNet( m_strName.c_str() ) )
	{
		strName += pszPrefix;
		strName += "_";
	}
	strName += m_strName;
	//printf("New net = %s\n", strName.c_str());
	CNet* pNet = pNetSection->GetNet(strName.c_str() );
	if ( !pNet )
	{
		pNet = new CNet( strName.c_str(), m_strRest.c_str());
		pNetSection->Add(pNet);
	}
	std::vector<CPin*>::iterator itP;
	int nSeq = pNet->PinCount();
	int nPinOffset = pNet->PinCount();
	for( itP = m_vecPins.begin(); itP != m_vecPins.end(); itP++ )
	{
		CPart* pPart = pPartsSection->GetPart( (*itP)->Part() );
		if ( pPart )
		{
			//printf("pPart was %x\n", pPart);
			//printf("Part ID %s\n", pPart->ID().c_str());
			CPin* pPin = new CPin( nSeq, pPart->ID().c_str(), (*itP)->Pin() );
			pNet->Add(pPin);
			nSeq++;
		}
		else
		{
			printf("Part [%s] not found\n", (*itP)->Part());
		}
	}
	std::vector<CConnect*>::iterator itC;
	for( itC = m_vecConnect.begin(); itC != m_vecConnect.end(); itC++ )
	{
		int nStartPin = (*itC)->StartPin();
		if ( nStartPin >= 0 )
		{
			nStartPin += nPinOffset;
		}
		int nEndPin = (*itC)->EndPin();
		if ( nEndPin >= 0 )
		{
			nEndPin += nPinOffset;
		}
		CConnect* pConnect = new CConnect((*itC)->Rest(), (*itC)->Sequence(), nStartPin, nEndPin ); 
		(*itC)->Append(nX, nY, pConnect);
		pNet->Add(pConnect);
	}
	std::vector<CArea*>::iterator itA;
	for( itA = m_vecAreas.begin(); itA != m_vecAreas.end(); itA++ )
	{
		CArea* pArea = new CArea( (*itA)->Area(), (*itA)->Sequence() );
		(*itA)->Append(nX, nY, pArea);
		pNet->Add(pArea);
	}
}
void CNet::Add(CPin* pPin)
{
	m_vecPins.push_back(pPin);
	pPin->SetSequence( m_vecPins.size() );
}
void CNet::Add(CConnect* pConnect)
{
	m_vecConnect.push_back(pConnect);
	pConnect->SetSequence( m_vecConnect.size() );
}
void CNet::Add(CArea* pArea)
{
	m_vecAreas.push_back(pArea);
	pArea->SetSequence( m_vecAreas.size() );
}
void CNet::Write(int nFile)
{
	std::string strNet = "net: \"";
	strNet += m_strName;
	strNet += "\" ";
	char sz[32];
	sprintf(sz,"%d %d %d", m_vecPins.size(), m_vecConnect.size(), m_vecAreas.size());
	strNet += sz;
	strNet += m_strRest;
	strNet += "\r\n";
	write(nFile, strNet.c_str(), strNet.length());
	std::vector< CPin* >::iterator itP;
	for( itP = m_vecPins.begin(); itP != m_vecPins.end(); itP++)
	{
		(*itP)->Write(nFile);
	}
	std::vector<CConnect*>::iterator itC;
	for( itC = m_vecConnect.begin(); itC != m_vecConnect.end(); itC++ )
	{
		(*itC)->Write(nFile);
	}
	std::vector<CArea*>::iterator itA;
	for( itA = m_vecAreas.begin(); itA != m_vecAreas.end(); itA++ )
	{
		(*itA)->Write(nFile);
	}
}
bool CNet::HasContent()
{
	if ( m_vecPins.size() > 0 || m_vecConnect.size() > 0 || m_vecAreas.size() > 0 )
	{
		return true;
	}
	return false;
}
int CNet::PinCount(){ return m_vecPins.size(); }