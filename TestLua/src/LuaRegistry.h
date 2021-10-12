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

// storing state in C functions

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	// 手动指定key，无法保证唯一性，需要小心维护
	lua_pushstring(L, "hongfei");
	lua_setfield(L, LUA_REGISTRYINDEX, "key");
	lua_getfield(L, LUA_REGISTRYINDEX, "key");
	stack_dump(L);
	lua_pop(L, 1);

	// 法1：reference system 生成唯一索引
	lua_pushstring(L, "james");
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	stack_dump(L);
	lua_pop(L, 1);

	// 法2：use the address of a static variable in our code as key
	static char key = 'k';
	
	// store a string
	lua_pushlightuserdata(L, (void*)&key);
	lua_pushstring(L, "bobo");
	lua_settable(L, LUA_REGISTRYINDEX);
	// retrieve a string
	lua_pushlightuserdata(L, (void*)&key);
	lua_gettable(L, LUA_REGISTRYINDEX);
	stack_dump(L);
	lua_pop(L, 1);

	lua_pushstring(L, "runyi");
	lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)&key);
	lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)&key);
	stack_dump(L);
}

/*

--stack_dump begin--
'hongfei'
--stack_dump end--
--stack_dump begin--
'james'
--stack_dump end--
--stack_dump begin--
'bobo'
--stack_dump end--
--stack_dump begin--
'runyi'
--stack_dump end--

G:\Projects\RunLuaCode\Win32\Debug\TestLua.exe (process 38168) exited with code 0.
Press any key to close this window . . .

*/