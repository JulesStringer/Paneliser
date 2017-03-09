#ifndef _FOOTPRINT_SECTION_INCLUDED
#define _FOOTPRINT_SECTION_INCLUDED 0
#include "section.h"
#include <map>

class CFootprintSection : public CSection
{
	std::map< std::string, std::string > m_mapFootprints;
	std::string m_strCurrentName;
	std::string m_strCurrentPrint;
public:
    CFootprintSection();
	void Merge(CFootprintSection* pOther);
	virtual void Write( const char* pszName, int nFile );
	virtual void ProcessLine( const char* pszLine );
	void Add( const char* pszName, const char* pszContent );
	virtual void Flush();
};

#endif