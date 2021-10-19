#pragma once
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

void test()
{
	lua_State* L = luaL_newstate();
	lua_State* L1 = lua_newthread(L);

	luaL_openlibs(L1);
	load_lua_src(L1, "lua/functions.lua");
	lua_getglobal(L1, "add_one");
	lua_pushinteger(L1, 5);
	lua_call(L1, 1, 1);
	printf("before xmove\n");
	stack_dump(L);
	lua_xmove(L1, L, 1);
	printf("after xmove\n");
	stack_dump(L);
}

void testCoroutine()
{
	lua_State* L = luaL_newstate();
	lua_State* L1 = lua_newthread(L);
	luaL_openlibs(L1);
	load_lua_src(L1, "lua/yield_example.lua");
	lua_getglobal(L1, "fool");
	lua_pushinteger(L1, 20);
	int nresults = 0;
	int ret = lua_resume(L1, L, 1, &nresults);
	printf("lua_resume return = %d\n", ret);	// LUA_YIELD
	printf("nresults = %d\n", nresults);		// 2
	stack_dump(L1); // [10 - 21]

	ret = lua_resume(L1, L, 0, &nresults);
	printf("lua_resume return = %d\n", ret);	// LUA_OK
	printf("nresults = %d\n", nresults);		// 1
	stack_dump(L1);	// [3]
}