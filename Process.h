#ifndef __PROCESS_INCLUDED
#define __PROCESS_INCLUDED 0

class CBoard;
#include <vector>
#include <string>

class CProcess
{
	std::vector<CBoard* > m_vecBoards;
	std::vector<std::string> m_vecNetsToMerge;
public:
    CProcess();
	int Process(int args, char** argv);
	bool MergeNet( const char* pszNet );
};
#endif
