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

// An implementation of tuples using upvalues

//x = tuple.new(10, "hi", {}, 3)
//print(x(1))-- > 10
//print(x(2))-- > hi
//print(x())-- > 10 hi table : 0x8087878 3

int t_tuple(lua_State* L)
{
	lua_Integer op = luaL_optinteger(L, 1, 0);
	if (op == 0)	// no arguments?
	{
		int i;
		// push each valid upvalue onto the stack
		for (i = 1; !lua_isnone(L, lua_upvalueindex(i)); ++i)
		{
			lua_pushvalue(L, lua_upvalueindex(i));
		}
		return i - 1;
	}
	else
	{
		// get field 'op'
		luaL_argcheck(L, 0 < op && op < 256, 1, "index out of range");
		if (lua_isnone(L, lua_upvalueindex(op)))	// no such field
			return 0;
		lua_pushvalue(L, lua_upvalueindex(op));
		return 1;
	}
}

int t_new(lua_State* L)	
{
	int top = lua_gettop(L);
	luaL_argcheck(L, top < 256, top, "too many fields");
	lua_pushcclosure(L, t_tuple, top);
	return 1;
}

// Make a C Module
static const struct luaL_Reg tuplelib[] = {
	{"new", t_new},
	{NULL, NULL},
};

int luaopen_tuplelib(lua_State* L)
{
	luaL_newlib(L, tuplelib);
	return 1;
}

void require_tuplelib(lua_State* L)
{
	// https://www.lua.org/manual/5.4/manual.html#luaL_requiref
	luaL_requiref(L, "tuple", luaopen_tuplelib, 1);
	lua_pop(L, 1);
}

// test
void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_tuplelib(L);
	if (luaL_dofile(L, "lua/tuple_test.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

/*

10
hi
10      hi      table: 0000026D74D29FE0 3

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 12808)已退出，代码为 0。
按任意键关闭此窗口. . .

*/

