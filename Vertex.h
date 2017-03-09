#ifndef _VERTEX_DEFINED
#define _VERTEX_DEFINED 0

#include <string>

class CVertex
{
	std::string m_strSegment;
	std::string m_strRest;
	int m_nIndex;
	int m_nX;
	int m_nY;
public:
	CVertex();
	CVertex( const char* psz);
	CVertex(int nX, int nY, int nIndex, const char* pszRest, const char* pszSegment );
	void ProcessLine( const char* psz);
	int X();
	int Y();
	int Index();
	const char* Rest();
	const char* Segment(); 
	void Write(int nFile);
};
#endif
