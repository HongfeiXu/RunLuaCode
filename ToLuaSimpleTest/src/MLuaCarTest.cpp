#include <stdio.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int luaopen_Car(lua_State* tolua_S);

void testMLuaCar()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	luaopen_Car(L);

	if (luaL_dofile(L, "src/MLuaCarTest.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

/*

100.0
table: 009917A0
.set    table: 00991750
.get    table: 00991520
200.0

E:\Projects\LUA\RunLuaCode\Win32\Debug\ToLuaSimpleTest.exe (process 55896) exited with code 0.
Press any key to close this window . . .

*/
