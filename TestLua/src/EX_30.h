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

// Exercise 30.1: Implement a filter function in C.It should receive a list 
// and a predicate and return a new
// list with all elements from the given list that satisfy the predicate :
// t = filter({ 1, 3, 20, -4, 5 }, function(x) return x < 5 end)
//	 --t = { 1, 3, -4 }

int lua_CCode_filter(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TFUNCTION);

	lua_newtable(L);	// table-func-table_new
	lua_insert(L, 2);	// table-table_new-func

	int n = luaL_len(L, 1);
	int j = 1;	// ÐÂlistÏÂ±ê
	for (int i = 1; i <= n; ++i)
	{
		lua_pushvalue(L, 3);	// table-table_new-func-func
		lua_rawgeti(L, 1, i);	// table-table_new-func-func-t[i]
		lua_pushvalue(L, -1);	// table-table_new-func-func-t[i]-t[i]
		lua_insert(L, -3);		// table-table_new-func-t[i]-func-t[i]
		lua_call(L, 1, 1);		// table-table_new-func-t[i]-result
		//stack_dump(L);
		if (lua_toboolean(L, -1))
		{
			lua_pop(L, 1);
			lua_rawseti(L, 2, j++);	// table-table_new-func
		}
		else
		{
			lua_pop(L, 2);
		}
	}
	lua_pop(L, 1);
	return 1;
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushcfunction(L, lua_CCode_filter);
	lua_setglobal(L, "CCode_filter");

	int errcode = luaL_dostring(L, "t = CCode_filter({1, 3, 20, -4, 5}, function (x) return x < 5 end)");
	if (errcode != LUA_OK)
	{
		simple_error_handler(L, "%s\n", lua_tostring(L, -1));
	}

	luaL_dostring(L, "for i = 1, #t do print(t[i]) end");

	lua_close(L);
}

/*

1
3
-4

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 44928) exited with code 0.
Press any key to close this window . .

*/

