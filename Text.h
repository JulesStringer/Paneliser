#ifndef _TEXT_INCLUDED
#define _TEXT_INCLUDED

#include <string>

class CText
{
	std::string m_strText;
	int32_t m_nX;
	int32_t m_nY;
	std::string m_strRest;
public:
	CText();
	CText(const char* pszText);
	CText(int nX, int nY, const char* pszText, const char* pszRest );
	void Write(int nFile);
	int X();
	int Y();
	const char* Text();
	const char* Rest();
    bool Valid();
};

#endif
