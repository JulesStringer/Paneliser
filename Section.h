#ifndef __SECTION_INCLUDED
#define __SECTION_INCLUDED

#include <vector>
#include <string>

class CSection
{
protected:
	std::vector< std::string > m_vecLines;
public:
	CSection();
	CSection(CSection* pFrom);
	static CSection* Create(const char* pszName);
	virtual void ProcessLine( const char* pszLine );
	virtual void Write( const char* pszName, int nFile );
	virtual void Flush();
};
#endif