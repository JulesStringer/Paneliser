#ifndef _PART_INCLUDED
#define _PART_INCLUDED 0

#include <string>
#include <vector>

class CPart
{
	int m_nNumber;
	std::string m_strType;
	int m_nX;
	int m_nY;
	std::string m_strPosRest;
	std::string m_strText;
        int m_bUnique;
        std::string m_strName;
public:
    CPart();
	CPart(const char* pszName);
    void Write( int nFile );
	void ProcessLine( const char* pszLine );
	const char* Type();
	void ReNumber(int nNo);
	std::string ID();
	void Offset( int nX, int nY);
    void SetUnique(int nUnique);
    std::string Name();
};

#endif
