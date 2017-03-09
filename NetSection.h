#ifndef _NETS_SECTION
#define _NETS_SECTION 0

#include "Section.h"

class CPartsSection;
class CNet;
class CProcess;

class CNetsSection : public CSection
{
	std::vector<CNet*> m_vecNets;
	CNet* m_pNet;
public:
    CNetsSection();
	void Append( int nX, int nY, const char* pszPrefix, CNetsSection* pNetsSection, CPartsSection* pPartsSection, CProcess* pProcess );
	virtual void Write( const char* pszName, int nFile );
	virtual void ProcessLine( const char* pszLine );
	virtual void Flush();
	void Add(CNet* pNet);
	CNet* GetNet(const char* pszNet);
};

#endif
