#pragma once

// Refs: Programming in Lua, 4th ed. Ch30 Techniques for Writing C Functions

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

// Splitting a string
// split a string according to a given separator
// and returns a table with the substring

static int lua_CCode_split(lua_State* L)
{
	const char* s = luaL_checkstring(L, 1);
	const char* sep = luaL_checkstring(L, 2);
	const char* e;

	lua_newtable(L);
	// repeat for each separator
	int i = 1;
	while ((e = strchr(s, *sep)) != NULL)
	{
		lua_pushlstring(L, s, e - s);	// push substring
		lua_rawseti(L, -2, i++);		// insert it in table(we know it has no metatable, so we can manipulate it with the raw operations)
		s = e + 1;						// skip separator
	}
	if (*s != '\0')	// "abc;123;" skip this, or it will push nil and set to table
	{
		lua_pushstring(L, s);	// insert last substring
		lua_rawseti(L, -2, i);
	}

	return 1;
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_split);
	lua_setglobal(L, "CCode_split");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "t = CCode_split(\"hello;world;fuck;\", \";\")");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	printf("------------------by Lua--------------------\n");
	luaL_dostring(L, "for i = 1, #t do print('--'..t[i]) end");
	
	printf("------------------by CAPI-------------------\n");
	lua_getglobal(L, "t");
	traverse_table(L, -1);


	lua_close(L);
}

/*

------------------by Lua--------------------
-hello
-world
-fuck
------------------by CAPI-------------------
'hello'
'world'
'fuck'

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 33600)已退出，代码为 0。
按任意键关闭此窗口. . .

*/
