#ifndef _AREA_INCLUDED
#define _AREA_INCLUDED 0

#include <string>
#include <vector>

class CCorner;

class CArea
{
	std::string m_strArea;
	std::vector< CCorner* > m_vecCorner;
	int m_nSequence;
public:
	CArea();
	CArea(const char * psz);
	CArea(const char * psz, int nSequence);
	void ProcessLine( const char* psz);
	const char* Area();
	void Append(int nX, int nY, CArea* pArea);
	void Write(int nFile);
	void Add(CCorner* pCorner);
	void SetSequence(int nSeq);
	int Sequence();
};
#endif
