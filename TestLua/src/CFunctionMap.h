#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int lua_CCode_map(lua_State* L)
{
	lua_Integer i, n;
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	n = luaL_len(L, 1);
	for (i = 1; i <= n; ++i)
	{
		lua_pushvalue(L, 2);	// push f
		lua_geti(L, 1, i);		// push t[i]
		lua_call(L, 1, 1);		// call f(t[i])
		lua_seti(L, 1, i);		// t[i] = result
	}
	return 0;
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_map);
	lua_setglobal(L, "CCode_map");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "t = {-1,-2,-3}");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	errcode = luaL_dostring(L, "CCode_map(t, math.abs)");
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	luaL_dostring(L, "for i = 1, #t do print(t[i]) end");

	lua_close(L);
}