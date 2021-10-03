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
	int errcode = luaL_dostring(L, "result_table = CCode_dir(\"..\")");	// ���ַ���ת��Ϊlua�ļ�������ִ��
	// ���������Ϣ
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}
	// ��ʽ1�� Lua ������ table ��Ϊ���б���ʽ�ģ���������ֱ�����
	printf("------------------by Lua--------------------\n");
	luaL_dostring(L, "for i = 1, #result_table do print(result_table[i]) end");

	// ��ʽ2��C API ������ table
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

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (���� 21520)���˳�������Ϊ 0��
��������رմ˴���. . .

*/

