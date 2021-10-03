#pragma once

// Refs: Programming in Lua, 4th ed. Ch29 Calling C from Lua

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


// define a lua_CFunction
int lua_CCode_sin(lua_State* L)
{
	double d = luaL_checknumber(L, 1);		// get argument, check tye type of its argument first
											// in case of error, it throw an informative error message
	lua_pushnumber(L, sin(d));				// push result
	return 1;
}

void testCFunctions()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	// register C Functions to Lua
	lua_pushcfunction(L, lua_CCode_sin);	// push a value of type function
	lua_setglobal(L, "CCode_sin");			// assgin it to the global variable mysin

	// 等价于上面两行
	//lua_register(L, "CCode_sin", lua_CCode_sin);


	// run this Lua C Function
	int errcode = luaL_dostring(L, "print(\"lua CCode_sin:\"..CCode_sin(1.57))");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

/*
	int errcode = luaL_dostring(L, "print(\"lua CCode_sin:\"..CCode_sin(1.57))");	// 将字符串转换为lua文件并解析执行


lua CCode_sin:0.99999968293183

	int errcode = luaL_dostring(L, "print(\"lua CCode_sin:\"..CCode_sin('a'))");	// 将字符串转换为lua文件并解析执行

[string "print("lua CCode_sin:"..CCode_sin('a'))"]:1: bad argument #1 to 'CCode_sin' (number expected, got string)

*/

