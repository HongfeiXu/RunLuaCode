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

// �ӱ�dll�����ķ���
LUALIB_API int luaopen_MyLib(lua_State* L)
{
	luaL_newlib(L, MyLib);
	return 1;
}


// ����LuaExe���̣�������lua�������е���
// ע��LuaExe.exe��MyLib.dll��ͬһ���ļ����£��������е�ʱ��ᶯ̬����

/*

Lua 5.4.3  Copyright (C) 1994-2021 Lua.org, PUC-Rio
> require("MyLib")
table: 000001CA4F514DA0 E:\BooksLearn\RunLuaCode\x64\Debug\MyLib.dll
> t = require("MyLib")
> t["add1"](9)
10.0
>

*/