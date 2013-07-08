#include "ACString.h"

BEGIN_NS_AC

void SplitString(const std::string& strSrc, std::vector<std::string>& vtDst, char cSep)
{
	if(strSrc.empty())
		return ;

	std::string::size_type spos = 0;
	std::string::size_type epos = 0;

	while((epos = strSrc.find_first_of(cSep, spos)) != std::string::npos)
	{
		std::string strTemp = strSrc.substr(spos, epos - spos);

		vtDst.push_back(strTemp);
		spos = epos + 1;
	}    

	vtDst.push_back(&strSrc[spos]);
};

CString StlStringToCString(const std::string& stlstr)
{
	int nLength = stlstr.length();
	TCHAR* pszBuffer = new TCHAR[nLength + 1];
	mbstowcs(pszBuffer, stlstr.c_str(), nLength + 1);
	CString cstr = pszBuffer;
	delete[] pszBuffer;
	return cstr;
}

std::string CStringToStlString(const CString& cstr)
{
	int nLength = wcslen(cstr.GetString()) * 2;
	char* pszBuffer = new char[nLength + 1];
	wcstombs(pszBuffer, cstr.GetString(), nLength + 1);
	std::string stlstr = pszBuffer;
	delete[] pszBuffer;
	return stlstr;
}

END_NS_AC