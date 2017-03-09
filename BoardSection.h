#ifndef _BOARD_SECTION_INCLUDED_
#define _BOARD_SECTION_INCLUDED_ 0

#include "Section.h"

class CBoard;

class CBoardSection : public CSection
{
	std::vector< CBoard* > m_vecBoards;
public:
    CBoardSection();
    void Add( CBoard* pBoard );
	virtual void Write( const char* pszName, int nFile);
};
#endif