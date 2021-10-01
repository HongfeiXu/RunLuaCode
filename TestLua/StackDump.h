#pragma once

// Refs: Programming in Lua, 4th ed. Ch27 Querying elements
// Deumping the stack

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
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