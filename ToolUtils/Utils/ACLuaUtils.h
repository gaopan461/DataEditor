#ifndef _AC_LUA_UTILS_H_
#define _AC_LUA_UTILS_H_

#include <lua.hpp>
#include <string>
#include <vector>
#include "ACDef.h"

BEGIN_NS_AC

class LuaConfig
{
public:
	LuaConfig();
	~LuaConfig();
public:
	int Load(const std::string& strFileName);
public:
	int GetInteger(const std::string& strParamName);
	std::string GetString(const std::string& strParamName);
	bool GetBoolean(const std::string& strParamName);
public:
	inline lua_State* GetLuaState()
	{
		return m_pLua;
	}
public:
	void PushTable(const std::string& strTableName);
private:
	int PushParam(const std::string& strParamName);
private:
	lua_State* m_pLua;
};

END_NS_AC

#endif//_AC_LUA_UTILS_H_