#include <stdio.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int tolua_Bindlua_open(lua_State* tolua_S);

extern double a;

void testBindLua()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	tolua_Bindlua_open(L);

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
}
