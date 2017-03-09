#ifndef __NET_DEFINED
#define __NET_DEFINED

#include <string>
#include <vector>

class CConnect;
class CPin;
class CArea;
class CNetsSection;
class CPartsSection;
class CProcess;

class CNet
{
	std::string m_strName;
	std::string m_strRest;
	CConnect* m_pConnect;
	CArea* m_pArea;
	std::vector<CPin*> m_vecPins;
	std::vector<CConnect*> m_vecConnect;
	std::vector<CArea*> m_vecAreas;
public:
	CNet();
	CNet( const char* pszLine );
	CNet( const char* pszName, const char* pszRest);
    void ProcessLine( const char* pszLine );
	const char* Name();
	void Append(int nX, int nY, const char* pszPrefix, CNetsSection* pNetSection, CPartsSection* pPartsSection, CProcess* pProcess);
	void Write(int nFile);
	void Add(CPin* pPin);
	void Add(CConnect* pConnect);
	void Add(CArea* pArea);
	bool HasContent();
	int PinCount();
};
#endif
