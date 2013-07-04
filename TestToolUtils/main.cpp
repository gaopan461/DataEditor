#include <iostream>
#include <cassert>
#include <lua.hpp>
#include <vector>
#include <string>
#include "ACLuaUtils.h"

USE_NS_AC

int LuaErrorFunc(lua_State* L)
{
	std::cout << lua_tostring(L,-1) << std::endl;
	assert(false);
	return 0;
}

void SplitString( const std::string& strSrc ,std::vector<std::string>& vtDst ,char cSep )
{
	if( strSrc.empty() )
		return ;

	std::string::size_type spos = 0;
	std::string::size_type epos = 0;

	while( (epos = strSrc.find_first_of( cSep ,spos)) != std::string::npos)
	{
		std::string strTemp=  strSrc.substr(spos , epos-spos );

		vtDst.push_back(strTemp);
		std::cout << "string = " << strTemp << std::endl;
		spos = epos + 1;
	}    

	vtDst.push_back(&strSrc[spos]);
	std::cout << "end string = "<< &strSrc[spos] << std::endl;
};

/*int main()
{
	lua_State* pLua = luaL_newstate();
	luaL_openlibs(pLua);

	lua_atpanic(pLua,&LuaErrorFunc);

	int ret = luaL_dofile(pLua,"./test.lua");
	if(ret != 0)
	{
		std::cout << lua_tostring(pLua,-1) << std::endl;
		return -1;
	}

	lua_getglobal(pLua,"a");
	int a = luaL_checkinteger(pLua,-1);

	std::cout << "Preview top:" << lua_gettop(pLua) << std::endl;

	int oldTop = lua_gettop(pLua);
	lua_pushinteger(pLua,100);
	lua_pushinteger(pLua,101);
	lua_insert(pLua,oldTop + 1);
	lua_settop(pLua,oldTop + 1);

	std::cout << "Current top:" << lua_gettop(pLua) << std::endl;
	std::cout << "top value:" << luaL_checkinteger(pLua,-1) << std::endl;

	lua_close(pLua);

	std::vector<std::string> vtDst;
	SplitString("./gao/pan",vtDst,'/');
	SplitString("/mei/yan",vtDst,'/');
	SplitString(".",vtDst,'/');

	char* buf = new char[2];
	buf[0] = 'a';
	buf[1] = '\0';
	std::string str = buf;
	delete [] buf;

	char* buf2 = new char[2];
	buf2[0] = 'b';
	buf2[1] = '\0';
	std::string str2 = buf2;
	delete [] buf2;

	std::cout << "str = " << str << std::endl;
	std::cout << "str2 = " << str2 << std::endl;
	return 0;
}*/

int main()
{
	LuaConfig luaConfig("./test.lua");

	int gInt = luaConfig.GetInteger("/gInt");
	std::cout << "gInt = " << gInt << std::endl;

	std::string gStr = luaConfig.GetString("/gStr");
	std::cout << "gStr = " << gStr << std::endl;

	int tableInt = luaConfig.GetInteger("/TestTable/nVal");
	std::cout << "tableInt = " << tableInt << std::endl;

	std::string tableStr = luaConfig.GetString("/TestTable/strVal");
	std::cout << "tableStr = " << tableStr << std::endl;

	std::cout << "Current top:" << lua_gettop(luaConfig.GetLuaState()) << std::endl;

	int tableInt2 = luaConfig.GetInteger("/TestTable/InTable/nVal");
	std::cout << "tableInt = " << tableInt2 << std::endl;

	std::string tableStr2 = luaConfig.GetString("/TestTable/InTable/strVal");
	std::cout << "tableStr = " << tableStr2 << std::endl;

	luaConfig.PushTable("/TestTable/InTable");
	std::cout << "Current top:" << lua_gettop(luaConfig.GetLuaState()) << std::endl;

	int tableInt3 = luaConfig.GetInteger("./nVal");
	std::cout << "tableInt = " << tableInt3 << std::endl;

	std::string tableStr3 = luaConfig.GetString("./strVal");
	std::cout << "tableStr = " << tableStr3 << std::endl;

	lua_pop(luaConfig.GetLuaState(),1);
	std::cout << "Current top:" << lua_gettop(luaConfig.GetLuaState()) << std::endl;

	return 0;
}