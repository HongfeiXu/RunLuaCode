#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "LuaUtils.h"

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
	lua_pushlstring(L, "102", 3);
	lua_pushboolean(L, 1);
	stack_dump(L);
	lua_close(L);
}
