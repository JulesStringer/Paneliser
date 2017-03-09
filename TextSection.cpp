#include "TextSection.h"
#include "Text.h"

#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

CTextSection::CTextSection()
{
}
void CTextSection::ProcessLine(const char* pszLine)
{
    if ( strncmp( pszLine, "text:", 5) == 0 )
	{
		CText* pText = new CText( pszLine + 6);
		m_vecTexts.push_back(pText);
//		printf("%s\n", pszLine);
	}
}
void CTextSection::Append(int nX, int nY, CTextSection* pTextSection)
{
	std::vector<CText*>::iterator it;
	for( it = m_vecTexts.begin(); it != m_vecTexts.end(); it++ )
	{
	    if ( (*it)->Valid() )
        {

            int X = nX + (*it)->X();
            int Y = nY + (*it)->Y();
            CText* pText = new CText(X, Y, (*it)->Text(), (*it)->Rest());
            pTextSection->Add(pText);
    //		printf("Appending %s\n", pText->Text());
        }
	}
}
void CTextSection::Write(const char* pszName, int nFile)
{
	printf("Writing text section\n");
	std::string strName(pszName);
	strName += "\r\n";
	write(nFile, strName.c_str(), strName.length() );
	std::vector<CText*>::iterator it;
	for( it = m_vecTexts.begin(); it != m_vecTexts.end(); it++ )
	{
//		printf("Writing %s\n", (*it)->Text());
        if ( (*it)->Valid() )
		{
			(*it)->Write(nFile);
		}
	}
}
void CTextSection::Add(CText* pText)
{
	m_vecTexts.push_back(pText);
}
