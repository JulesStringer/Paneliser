#include "pin.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CPin::CPin()
{
	m_nSequence = 0;
	m_nPin = 0;
}
/*
  pin: 1 RJ45.7
  
  	int m_nSequence;
	std::string m_strPart;
	int m_nPin;

*/
CPin::CPin(const char* psz)
{
//	printf("%s\n", psz );
	m_nSequence = 0;
	m_nPin = 0;
	while(isdigit(*psz) )
	{
		m_nSequence = 10 * m_nSequence + *psz - '0';
		psz++;
	}
	while( *psz == ' ' )
	{
		psz++;
	}
	while(*psz && *psz != '.' )
	{
		m_strPart += *psz;
		psz++;
	}
	if ( *psz == '.' )
	{
		psz++;
		while(isdigit(*psz))
		{
			m_nPin = 10 * m_nPin + *psz - '0';
			psz++;
		}
	}
	//printf("Pin seq = %d Name = [%s] Pin = %d\n", m_nSequence, m_strPart.c_str(), m_nPin );
}
CPin::CPin( int nSequence, const char* pszPart, int nPin)
{
	m_nSequence = nSequence;
	m_strPart = pszPart;
	m_nPin = nPin;
	//printf("New Pin sequence = %d part = %s pin = %d\n", nSequence, pszPart, nPin );
}
const char* CPin::Part()
{
	return m_strPart.c_str();
}
int CPin::Sequence() { return m_nSequence; }
int CPin::Pin() { return m_nPin; }
void CPin::Write(int nFile)
{
	char sz[256];
	sprintf(sz," pin: %d %s.%d\r\n", m_nSequence, m_strPart.c_str(), m_nPin );
	write(nFile, sz, strlen(sz) );
}
void CPin::SetSequence(int nSeq)
{
	m_nSequence = nSeq;
}

