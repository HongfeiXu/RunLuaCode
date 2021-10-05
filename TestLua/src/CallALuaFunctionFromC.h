#pragma once

// Refs: Programming in Lua, 4th ed. Ch28 Extending Your Application

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

// call a function 'f' defined in Lua
double call_lua_f(lua_State* L, double x, double y)
{
	int isnum;
	double z;

	lua_getglobal(L, "f");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	if (lua_pcall(L, 2, 1, 0) != LUA_OK)
	{
		simple_error_handler(L, "error running function 'f': %s", lua_tostring(L, -1));
	}
	z = lua_tonumberx(L, -1, &isnum);
	if (!isnum)
	{
		simple_error_handler(L, "function 'f' should return a number");
	}
	lua_pop(L, 1);
	return z;
}


void callALuaFunctionFromC()
{
	lua_State* L = luaL_newstate();
	
	luaL_openlibs(L);	// 打开指定状态机中的所有Lua标准库，
	// 否则会提示
	// 'functions.lua:2: attempt to index a nil value (global 'math')'

	load_lua_src(L, "functions.lua");
	
	double result = call_lua_f(L, 3, 4);
	printf("result = %lf\n", result);

	lua_close(L);
}

