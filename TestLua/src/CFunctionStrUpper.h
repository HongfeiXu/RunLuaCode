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

// The function string.upper

static int lua_CCode_str_upper(lua_State* L)
{
	size_t l;
	size_t i;
	luaL_Buffer b;
	const char* s = luaL_checklstring(L, 1, &l);
	char* p = luaL_buffinitsize(L, &b, l);
	for (i = 0; i < l; ++i)
	{
		p[i] = toupper(unsigned char(s[i]));
	}
	luaL_pushresultsize(&b, l);
	return 1;
}


void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_str_upper);
	lua_setglobal(L, "CCode_str_upper");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "s = CCode_str_upper(\"hello world\")");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	luaL_dostring(L, "print(s)");

	lua_close(L);
}

/*

HELLO WORLD

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 24412)已退出，代码为 0。
按任意键关闭此窗口. . .

*/