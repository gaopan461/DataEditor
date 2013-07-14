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
public:
	bool IsTopTable();
public:
	template <class T>
	void IterTable(const std::string& strTableName, T* obj, void(T::*func)())
	{
		PushTable(strTableName);

		int tableLen = lua_objlen(m_pLua,-1);
		for(int i = 1; i <= tableLen; ++i)
		{
			lua_pushinteger(m_pLua, i);
			lua_gettable(m_pLua, -2);

			(obj->*func)();

			lua_pop(m_pLua, 1);
		}

		lua_pop(m_pLua, 1);
	}
private:
	int PushParam(const std::string& strParamName);
private:
	lua_State* m_pLua;
};

END_NS_AC

#endif//_AC_LUA_UTILS_H_