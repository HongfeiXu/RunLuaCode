#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static int test_add1(lua_State* L)
{
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, d+1);
	return 1;
}

static int test_sub1(lua_State* L)
{
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, d - 1);
	return 1;
}

static const struct luaL_Reg MyLib[] = {
	{"add1", test_add1},
	{"sub1", test_sub1},
	{NULL, NULL},
};

// 从本dll导出的符号
LUALIB_API int luaopen_MyLib(lua_State* L)
{
	luaL_newlib(L, MyLib);
	return 1;
}


// 运行LuaExe工程，可以在lua解释器中导入
// 注：LuaExe.exe与MyLib.dll在同一个文件夹下，所以运行的时候会动态链接

/*

Lua 5.4.3  Copyright (C) 1994-2021 Lua.org, PUC-Rio
> require("MyLib")
table: 000001CA4F514DA0 E:\BooksLearn\RunLuaCode\x64\Debug\MyLib.dll
> t = require("MyLib")
> t["add1"](9)
10.0
>

*/