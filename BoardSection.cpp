#include "BoardSection.h"
#include "Board.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CBoardSection::CBoardSection()
{
}
void CBoardSection::Add( CBoard* pBoard )
{
	m_vecBoards.push_back(pBoard);
}
void CBoardSection::Write( const char* pszName, int nFile )
{
	std::string strName(pszName);
	strName += "\r\n";
	write(nFile, strName.c_str(), strName.length() );
	std::vector< CBoard* >::iterator it;
	int nSeq = 0;
	for( it = m_vecBoards.begin(); it != m_vecBoards.end(); it++, nSeq++ )
	{
		(*it)->WriteOutline( nFile, nSeq );
	}
}
