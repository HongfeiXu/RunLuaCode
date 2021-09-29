#pragma once

// Refs: Programming in Lua, 4th ed. Ch27 A First Example
// A bare-bones stand-alone Lua interpreter
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

void simple_lua_interpreter(void)
{
	char buff[256];
	int error;
	lua_State* L = luaL_newstate();	// opens Lua
	luaL_openlibs(L);				// opens the standard libraries
	while (fgets(buff, sizeof(buff), stdin) != NULL)
	{
		error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);			// pop error message from the stack
		}
	}
	lua_close(L);
}

void simple_error_handler(lua_State* L, const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}


