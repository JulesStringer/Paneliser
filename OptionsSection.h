#ifndef _OPTIONS_SECTION_INCLUDED
#define _OPTIONS_SECTION_INCLUDED 0
#include "section.h"

class COptionsSection : public CSection
{
public:
	virtual ~COptionsSection();
    COptionsSection();
    COptionsSection( COptionsSection* pOptionsSection);
	void Replace( const char* pszOption, const char* pszValue);
};
#endif
