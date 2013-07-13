#include "ToolConfig.h"
#include "ACString.h"

BEGIN_NS_AC

ToolConfig::ToolConfig(ToolApp* app)
: Module<ToolApp>(app)
{}

int ToolConfig::Initial(const CString& strAppName)
{
	CString cstrFileName = strAppName + ".lua";
	std::string stlstrFileName = CStringToStlString(cstrFileName);
	m_objLua.Load(stlstrFileName);
	return 0;
}

END_NS_AC