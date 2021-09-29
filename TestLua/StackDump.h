#pragma once

// Refs: Programming in Lua, 4th ed. Ch27 Querying elements
// Deumping the stack
// A simple error handler

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static void stack_dump(lua_State* L)
{
	int i;
	int top = lua_gettop(L);	// depth of the stack
	for (i = 1; i <= top; ++i)
	{
		int t = lua_type(L, i);
		switch (t)
		{
		case LUA_TSTRING:
		{
			printf("'%s'", lua_tostring(L, i));
			break;
		}
		case LUA_TBOOLEAN:
		{
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		}
		case LUA_TNUMBER:
		{
			// integer?
			if (lua_isinteger(L, i))
			{
				printf("%lld", lua_tointeger(L, i));
			}
			// float
			else
			{
				printf("%g", lua_tonumber(L, i));
			}
			break;
		}
		default:
			printf("%s", lua_typename(L, t));
			break;
		}
		printf("   ");
	}
	printf("\n");
}

void stack_dump_test()
{
	lua_State* L = luaL_newstate();
	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");

	stack_dump(L);
	// true   10   nil   'hello'

	lua_pushvalue(L, -4); stack_dump(L);
	// true   10   nil   'hello'   true

	lua_replace(L, 3); stack_dump(L);
	// true   10   true   'hello'

	lua_settop(L, 6);  stack_dump(L);
	// true   10   true   'hello'   nil   nil
	
	lua_rotate(L, 3, 1); stack_dump(L);
	// true   10   nil   'hello'   nil

	lua_remove(L, -3); stack_dump(L);
	// true   10   nil   'hello'   nil

	lua_settop(L, -5); stack_dump(L);
	// true

	lua_close(L);
}