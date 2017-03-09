#ifndef _TEXT_SECTION_INCLUDED
#define _TEXT_SECTION_INCLUDED 0

#include "section.h"

#include <vector>
class CText;

class CTextSection: public CSection
{
	std::vector< CText* > m_vecTexts;
public:
	CTextSection();
	virtual void ProcessLine( const char* pszLine );
	void Append(int nX, int nY, CTextSection* pTextSection);
    virtual void Write(const char* pszName, int nFile);
	void Add(CText* pText);
};
#endif
