#pragma once

// Refs: Programming in Lua, 4th ed. Ch30 Techniques for Writing C Functions

#include <stdio.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int counter(lua_State* L);	// forward declaration

static int newCounter(lua_State* L)
{
	lua_pushinteger(L, 0);				// push initial values for its upvalues on the stack
	lua_pushcclosure(L, &counter, 1);	// create a new closure
	return 1;
}

static int counter(lua_State* L)
{
	lua_Integer val = lua_tointeger(L, lua_upvalueindex(1));
	lua_pushinteger(L, ++val);				// new value
	lua_copy(L, -1, lua_upvalueindex(1));	// update upvalue
	return 1;
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, newCounter);
	lua_setglobal(L, "newCounter");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "c1 = newCounter(); c2 = newCounter()");
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	errcode = luaL_dostring(L, "print(c1(), c1(), c2(), c1())");
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	lua_close(L);
}

/*

1       2       1       3

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 46516)已退出，代码为 0。
按任意键关闭此窗口. . .

*/
