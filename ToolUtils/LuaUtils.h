#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

#include <lua.hpp>

int LuaCheckInteger(lua_State* L, int index)
{
	assert(lua_type(L,index) == LUA_TNUMBER);
	return luaL_checkinteger(L,index);
}

float LuaCheckFloat(lua_State* L, int index)
{
}

#endif//_LUA_UTILS_H_