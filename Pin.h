#ifndef _PIN_INCLUDED
#define _PIN_INCLUDED 0

#include <string>

class CPin
{
	int m_nSequence;
	std::string m_strPart;
	int m_nPin;
public:
	CPin();
	CPin(const char* psz);
	CPin( int nSequence, const char* pszPart, int nPin);
	const char* Part();
	int Sequence();
	int Pin();
	void Write(int nFile);
	void SetSequence(int nSeq);
};
#endif

