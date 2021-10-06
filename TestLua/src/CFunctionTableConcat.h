#pragma once

// Refs: Programming in Lua, 4th ed. Ch30 Techniques for Writing C Functions

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int lua_CCode_table_concat(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_Integer n = luaL_len(L, 1);

	luaL_Buffer b;
	luaL_buffinit(L, &b);
	for (int i = 1; i <= n; ++i)
	{
		lua_geti(L, 1, i);
		luaL_addvalue(&b);		// Adds the value on the top of the stack to the buffer B. Pops the value.
	}
	luaL_pushresult(&b);
	return 1;
}


void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_table_concat);
	lua_setglobal(L, "CCode_table_concat");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "s = CCode_table_concat({'hello', 123, 'world'})");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	luaL_dostring(L, "print(s)");

	lua_close(L);
}

/*

hello123world

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 28108)已退出，代码为 0。
按任意键关闭此窗口. . .

*/