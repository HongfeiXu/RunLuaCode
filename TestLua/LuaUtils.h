#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


// a simple error handler, 
// prints an error message, closes the Lua state, finishes the whole application
void simple_error_handler(lua_State* L, const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

// Deumping the stack
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

// 加载lua源文件，并运行一下
void load_lua_src(lua_State* L, const char* fname)
{
	// luaL_loadfile 底层走的是 lua_load，
	// 如果没有错误，将编译好的代码块压到栈顶
	// 否则，将错误消息压到栈顶
	// lua_pcall run the compiled chunk
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
	{
		simple_error_handler(L, "cannot run config. file: %s", lua_tostring(L, -1));
	}
}
