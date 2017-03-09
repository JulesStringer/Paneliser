#ifndef _CORNER_DEFINED_
#define _CORNER_DEFINED_

#include <string>

class CCorner
{
	int m_nIndex;
	int m_nX;
	int m_nY;
	std::string m_strRest;
public:
	CCorner();
	CCorner(const char* psz);
	CCorner(int X, int Y, int nIndex, const char* pszRest);
	int X();
	int Y();
	const char* Rest();
	void Write(int nFile);
	/*
	corner: <index of first corner> <x> <y> <style of next side> <flag if last corner of this contour>
	*/
};

#endif