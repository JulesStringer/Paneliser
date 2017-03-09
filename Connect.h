#ifndef _CONNECT_DEFINED 
#define _CONNECT_DEFINED 0

#include <string>
#include <vector>

class CVertex;

class CConnect
{
	std::string m_strRest;
	std::vector< CVertex*> m_vecVertices;
	CVertex* m_pVertex;
	int m_nSequence;
	int m_nStartPin;
	int m_nEndPin;
public:
	CConnect();
	CConnect(const char* pszRest);
	CConnect(const char* pszRest, int nSequence, int nStartPin, int nEndPin);
	void ProcessLine( const char* pszLine );
	const char* Rest();
	void Append(int nX, int nY, CConnect* pConnect);
	void Write(int nFile);
	void Add(CVertex* pVertex);
	void SetSequence(int nSeq);
	int Sequence();
	int StartPin();
	int EndPin();
};
#endif 
