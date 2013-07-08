#ifndef _AC_STRING_H_
#define _AC_STRING_H_

#include <string>
#include <vector>
#include "ACDef.h"

BEGIN_NS_AC

void SplitString( const std::string& strSrc ,std::vector<std::string>& vtDst ,char cSep );
CString StlStringToCString(const std::string& stlstr);
std::string CStringToStlString(const CString& cstr);

END_NS_AC

#endif