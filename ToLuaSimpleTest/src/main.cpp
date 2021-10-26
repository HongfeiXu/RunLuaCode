#include <stdio.h>

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

double a = 1;

int tolua_Bindlua_open(lua_State* tolua_S);

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	tolua_Bindlua_open(L);

	a = 99;
	if (luaL_dofile(L, "src/a.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	printf("a = %f\n\n\n", a);

	a = a + 1000;

	if (luaL_dofile(L, "src/a.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	printf("a = %f\n", a);

	lua_close(L);
	return 0;
}

/*

99.0
c=      nil
b=      adas
a = 100.000000


1100.0
c=      nil
b=      adas
a = 1101.000000

E:\BooksLearn\LUA\ToLuaLearn\Debug\ToLuaSimpleTest.exe (进程 82340)已退出，代码为 0。
按任意键关闭此窗口. . .

*/

