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

// Exercise 29.1: Write a variadic summation function, in C, 
// that computes the sum of its variable number of numeric argument

static int lua_CCode_summation(lua_State* L)
{
	int n = lua_gettop(L);
	lua_Number sum = 0.0;
	for (int i = 1; i <= n; ++i)
	{
		int isnumber;
		lua_Number curr_num;
		curr_num = lua_tonumberx(L, i, &isnumber);
		if (!isnumber)
		{
			simple_error_handler(L, "wrong param");
		}
		sum += curr_num;
	}
	lua_pushnumber(L, sum);
	return 1;
}

// Exercise 29.2: Implement a function equivalent to table.pack, 
// from the standard library.

static int lua_CCode_pack(lua_State* L)
{
	stack_dump(L);					// [a, b, c]
	int n = lua_gettop(L);
	lua_newtable(L);				// [a, b, c, table]
	stack_dump(L);
	lua_insert(L, 1);				// [table, a, b, c]
	stack_dump(L);
	for (int i = n; i >= 1; --i)
	{
		lua_pushinteger(L, i);		// [table, a, b,c, 3]
		lua_insert(L, -2);		// 将value放在栈顶，key放到value下面 [table, a, b, 3, c]
		stack_dump(L);
		lua_settable(L, 1);			// [table, a, b]
		stack_dump(L);
	}
	lua_pushstring(L, "n");
	lua_pushinteger(L, n);
	lua_settable(L, 1);
	return 1;
}


// Exercise 29.3: Write a function that takes any number of parameters and returns them in reverse order.
// print(reverse(1, "hello", 20))-- > 20 hello 1

static int lua_CCode_reverse(lua_State* L)
{
	int n = lua_gettop(L);
	for (int i = 1; i < n; ++i)
	{
		lua_insert(L, i);
	}
	return n;
}
// Exercise 29.4: Write a function foreach that takes a table and a function and calls that function for each
// key–value pair in the table.
// foreach({ x = 10, y = 20 }, print)
// -- > x 10
// -- > y 20
//(Hint: check the function lua_next in the Lua manual.)

static int lua_CCode_foreach(lua_State* L)
{
	int n = lua_gettop(L);
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TFUNCTION);
	lua_pushnil(L);
	while (lua_next(L, 1) != 0)
	{
		lua_pushvalue(L, -2);	// 复制键做下一次迭代
		lua_insert(L, -4);
		lua_pushvalue(L, 3);	// 复制函数
		lua_insert(L, 2);
		lua_pcall(L, 2, LUA_MULTRET, 0);	// 调用

		lua_settop(L, 3);	// 栈到余下三个元素的状态 table - func - key
	}
	lua_settop(L, 0);
	return 0;		// 无返回值
}


// Make a C Module

static const struct luaL_Reg exlib[] = {
	{"summation", lua_CCode_summation},
	{"pack", lua_CCode_pack},
	{"reverse", lua_CCode_reverse},
	{"foreach", lua_CCode_foreach},
	{NULL, NULL},
};

int luaopen_exlib(lua_State* L)
{
	luaL_newlib(L, exlib);
	return 1;
}

void require_exlib(lua_State* L)
{
	// https://www.lua.org/manual/5.4/manual.html#luaL_requiref
	luaL_requiref(L, "exlib", luaopen_exlib, 1);
	lua_pop(L, 1);
}


// test

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_exlib(L);
	if (luaL_dofile(L, "lua/EX_29.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}