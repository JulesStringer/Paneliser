#ifndef _BOARD_INCLUDED
#define _BOARD_INCLUDED

#include <string>
#include <map>

class CSection;
class CFootprintSection;
class CPartsSection;
class CNetsSection;
class CTextSection;
class CProcess;

class CBoard
{
	double m_dX;                 		//    X  - Position of board on combined panel (MM)
	double m_dY;                 		//    Y  - Position of board on combined panel (MM)
	double m_dCX;                		//    CX - Width of board on combined panel(MM)
	double m_dCY;                		//    CY - Height of board on combined panel(MM)
	std::string m_strPrefix;     	//    Prefix - Prefix to apply to part numbers and nets
	std::string m_strSourceFile; 	//    SourceFile - Source fpc file
    CSection* m_pSection;     		// section currently being processed.
	std::map<std::string, CSection*> m_mapSections;
	void ProcessLine( const char* pszLine );
	void WriteSection( const char* pszName, int nFile );
	CFootprintSection* m_pFootprintSection;
	bool m_bNoOutline;
public:
    CBoard();
	void SetNoOutline(bool bNoOutline);
	CBoard(const char* pszLine);
	CBoard(const char* pszFile, double dWidth, double dHeight);
	~CBoard();
	std::string ToString();
	void Read();
	CSection* GetSection(const char* pszName);
	void AddSection( const char* pszName, CSection* pSection);
	void Write();
	bool IsValid();
	void MergeFootprints( CFootprintSection* pFootprints );
    void WriteOutline( int nFile, int nSeq );
	void AppendParts( CPartsSection* pPartsSection );
	void AppendNets( CNetsSection* pNetsSection, CProcess* pProcess );
	void AppendTexts( CTextSection* pTextSection );
};

#endif