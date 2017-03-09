#include "Process.h"

#include <sys/types.h>
//#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "board.h"
#include "section.h"
#include "optionssection.h"
#include "footprintsection.h"
#include "boardsection.h"
#include "partssection.h"
#include "netsection.h"
#include "textsection.h"

CProcess::CProcess()
{
}
int CProcess::Process(int args, char** argv)
{
	// Parameters:
	// Boards file contains one line per board:
	//    X  - Position of board on combined panel (MM)
	//    Y  - Position of board on combined panel (MM)
	//    CX - Width of board on combined panel(MM)
	//    CY - Height of board on combined panel(MM)
	//    Prefix - Prefix to apply to part numbers and nets
	//    SourceFile - Source fpc file
	//
	int n = 0;
	double dWidth = 100.0;
	double dHeight = 100.0;
	std::string strBoards;
	std::string strOutput;
	std::string strCAM = "";
	bool bNoOutline = false;
	for( n = 1; n < args; n++ )
	{
		if ( strcmp(argv[n], "-b") == 0 )
		{
			n++;
	        strBoards = argv[n];
		}
		else if ( strcmp(argv[n], "-o") == 0 )
		{
			n++;
			strOutput = argv[n];
		}
		else if ( strcmp(argv[n], "-c") == 0 )
		{
			n++;
			strCAM = argv[n];
		}
		else if ( strcmp(argv[n], "-m") == 0 )
		{
			n++;
			std::string str = argv[n];
			m_vecNetsToMerge.push_back(str);
		}
		else if ( strcmp(argv[n], "-w") == 0 )
		{
			n++;
			dWidth = atof(argv[n]);
		}
		else if ( strcmp(argv[n], "-h") == 0 )
		{
			n++;
			dHeight = atof(argv[n]);
		}
		else if ( strcmp(argv[n], "-nooutline") == 0 )
		{
			bNoOutline = true;
		}
	}
	if ( strBoards.length() == 0 || strOutput.length() == 0 )
	{
		printf("Usage:-\n");
		printf("Panelise -b <boardsfile> -o <outputfile> -c <CAM> [options]\n");
		printf("    <boardsfile> - File containing one line per board describing board to be panelised \n");
		printf("    <outputFile> - fpc file containing combined board \n");
		printf("    <CAM> - folder to contain CAM files\n");
		printf("-b board definition\n");
		printf("-o output file\n");
		printf("-c CMA files path to include in output\n");
		printf("Options:\n");
		printf("-m names of net to merge - can have several of these\n");
		printf("-w width of board outline\n");
		printf("-h height of board outline\n");
		return 1;
	}
	// Read boards file.
	int nBFL = open( strBoards.c_str(), O_RDONLY );
	if ( nBFL == -1 )
	{
		printf("Unable to open board file %s\n", strBoards.c_str());
		return 2;
	}
	off_t len = lseek(nBFL, 0L, SEEK_END );
	lseek(nBFL, 0L, SEEK_SET );
	char* psz = new char[len+1];
	char* pszFile = psz;
	read( nBFL, psz, len);
	psz[len] = 0;
	close(nBFL);
	std::string strLine;
	while( *psz )
	{
		if ( *psz == '\r' || *psz == '\n' )
		{
			if ( strLine.length() > 0 )
			{
				CBoard* pBoard = new CBoard( strLine.c_str());
				printf("%s\n", pBoard->ToString().c_str() );
				if ( pBoard->IsValid() )
				{
					m_vecBoards.push_back(pBoard);
					printf("Board added\n");
				}
			}
			strLine = "";
		}
		else
		{
			strLine += *psz;
		}
		psz++;
	}
	delete [] pszFile;
	// Read each board
	printf("About to read boards\n");
	std::vector< CBoard* >::iterator itB;
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		(*itB)->Read();
		(*itB)->SetNoOutline(bNoOutline);
	}
	printf("Read Boards\n");
	CBoard* pCombined = new CBoard(strOutput.c_str(), dWidth, dHeight);
	printf("Created master board\n");
	// Use first available options section
	COptionsSection* pOptions = new COptionsSection( (COptionsSection*)m_vecBoards[0]->GetSection("[options]"));
	printf("Created options section\n");
	pOptions->Replace("project_name", "\"combined\"" );
	pOptions->Replace("SMT_connect_copper","\"1\"");
	printf("Replaced project name\n");
	pCombined->AddSection("[options]", pOptions );
	printf("About to create CAM\n");
	if ( strCAM.length() == 0 )
	{
		// create default from output path.
		strCAM = strOutput.c_str();
		int n = strCAM.length();
		while( n > 0 && strCAM[n] != '\\' )
		{
			n--;
		}
		strCAM.erase(n);
		strCAM += "\\CAM";
		printf("CAM Folder changed to %s\n", strCAM.c_str());
	}
	if ( strCAM.length() > 0 )
	{
		pOptions->Replace("CAM_folder", strCAM.c_str());
	}
	printf("Created CAM folder\n");
	// create merged footprints section
	CFootprintSection* pFootprints = new CFootprintSection();
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		(*itB)->MergeFootprints( pFootprints );
	}
	printf("Merged footprints\n");
	pCombined->AddSection("[footprints]", (CSection*)pFootprints);
	printf("Added footprints\n");
	//
	// Add [board]
	// Create board section - contains board outlines.
	//
	CBoardSection* pBoardSection = new CBoardSection();
	pBoardSection->Add( pCombined );
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		pBoardSection->Add( *itB );
	}
	pCombined->AddSection("[board]", pBoardSection );
	printf("Added board section\n");
	// Add [solder_mask_cutouts]
	// Add [parts] to existing parts section for each board with offsets
	CPartsSection* pPartsSection = new CPartsSection();
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		(*itB)->AppendParts( pPartsSection );
	}
        pPartsSection->SetUniqueness();
	pCombined->AddSection("[parts]", pPartsSection );
	printf("Added parts\n");
    // Add [nets] to existing nets section for each board with offsets
	CNetsSection* pNetsSection = new CNetsSection();
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		(*itB)->AppendNets( pNetsSection, this );
	}
	pCombined->AddSection("[nets]", pNetsSection );
	printf("Added nets\n");
    // Add [texts] to existing text section for each board with offsets
	CTextSection* pTextSection = new CTextSection();
	for( itB = m_vecBoards.begin(); itB != m_vecBoards.end(); itB++ )
	{
		(*itB)->AppendTexts( pTextSection );
	}
	pCombined->AddSection("[texts]", pTextSection );
	printf("Added texts\n");
	// Add [end] section
	CSection* pEnd = new CSection();
	pCombined->AddSection("[end]", pEnd );
	// Write out combined board.
	pCombined->Write();
	return 0;
}
bool CProcess::MergeNet( const char* pszNet )
{
	std::vector<std::string>::iterator it;
	for( it = m_vecNetsToMerge.begin(); it != m_vecNetsToMerge.end(); it++ )
	{
		if ( strcmp( (*it).c_str(), pszNet ) == 0 )
		{
			return true;
		}
	}
	return false;
}
