#include <iostream>
#include <cassert>
#include <lua.hpp>

int main()
{
	lua_State* pLua = luaL_newstate();
	luaL_openlibs(pLua);

	int ret = luaL_dofile(pLua,"./test.lua");
	if(ret != 0)
	{
		std::cout << lua_tostring(pLua,-1) << std::endl;
		return -1;
	}

	lua_getglobal(pLua,"a");
	luaL_checktype(pLua,-1,LUA_TNUMBER);
	int a = luaL_checkinteger(pLua,-1);

	lua_close(pLua);
	return 0;
}