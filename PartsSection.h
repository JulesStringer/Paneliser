#ifndef _PARTS_SECTION_INCLUDED
#define _PARTS_SECTION_INCLUDED 0

#include "section.h"
#include <map>

class CPart;
class CBoard;

class CPartsSection : public CSection
{
	std::map< std::string, CPart* > m_mapParts;
	std::map< std::string, int > m_mapPartNos;
	std::vector< CPart* > m_vecParts;
	CPart* m_pPart;
public:
    CPartsSection();
	virtual void Write( const char* pszName, int nFile );
	virtual void ProcessLine( const char* pszLine );
	virtual void Flush();
	CPart* GetPart(const char* pszName);
	void Add(CPart* pPart);
	void Append(int nX, int nY, CPartsSection* pSection );
	std::string ToString();
	void SetUniqueness();
};

#endif
