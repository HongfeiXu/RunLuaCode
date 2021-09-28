#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void test_lua_api(void)
{
	lua_State* L = luaL_newstate();	// opens Lua
	luaL_openlibs(L);
	lua_pushinteger(L, 10);
	printf("is string %d\n", lua_isstring(L, -1));
	lua_pushlstring(L, "102", 3);
	printf("is number %d\n", lua_isnumber(L, -1));
	printf("lua_type L[-1] = %d\n", lua_type(L, -1));
	printf("lua_toboolean L[-2] = %d\n", lua_toboolean(L, -2));
	size_t len;
	const char* s = lua_tolstring(L, -1, &len);
	printf("len = %lld, s[len] == %c", len, s[len]);
	lua_close(L);
}
