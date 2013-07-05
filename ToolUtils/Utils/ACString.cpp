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

CString StlStringToCString(const std::string& str)
{
	CString cstr = ("%s",str.c_str());
	return cstr;
}

END_NS_AC