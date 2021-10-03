#pragma once

// Refs: Programming in Lua, 4th ed. Ch29 Calling C from Lua

#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

// A function to read a directory
int lua_CCode_dir(lua_State* L)
{
	DIR* dir;
	struct dirent* entry;
	int i;
	const char* path = luaL_checkstring(L, 1);

	dir = opendir(path);
	if (dir == NULL)	// error opening the directory?
	{
		lua_pushnil(L);	// return nil...
		lua_pushstring(L, strerror(errno));	// and error message
		return 2;		// number of results
	}

	// create result table
	lua_newtable(L);
	i = 1;
	while ((entry = readdir(dir)) != NULL)
	{
		lua_pushinteger(L, i++);
		lua_pushstring(L, entry->d_name);
		lua_settable(L, -3);	// pops the key and the value from the stack
	}
	closedir(dir);

	return 1;
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_dir);	// push a value of type function
	lua_setglobal(L, "CCode_dir");

	// run this Lua C Function
	int errcode = luaL_dostring(L, "result_table = CCode_dir(\"..\")");	// 将字符串转换为lua文件并解析执行
	// 输出错误信息
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}
	// 方式1： Lua 输出这个 table 因为是列表形式的，可以这样直接输出
	printf("------------------by Lua--------------------\n");
	luaL_dostring(L, "for i = 1, #result_table do print(result_table[i]) end");

	// 方式2：C API 输出这个 table
	printf("------------------by CAPI-------------------\n");
	lua_getglobal(L, "result_table");
	traverse_table(L, -1);

	lua_close(L);
}

/*

------------------by Lua--------------------
.
..
.git
.gitattributes
.gitignore
.vs
LuaExe
LuaLib
README.md
RunLuaCode.sln
src
TestLua
thirdparty
x64
------------------by CAPI-------------------
'.'
'..'
'.git'
'.gitattributes'
'.gitignore'
'.vs'
'LuaExe'
'LuaLib'
'README.md'
'RunLuaCode.sln'
'src'
'TestLua'
'thirdparty'
'x64'

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 21520)已退出，代码为 0。
按任意键关闭此窗口. . .

*/

