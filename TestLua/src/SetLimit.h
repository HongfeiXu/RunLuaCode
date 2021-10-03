#pragma once

// Exercise 27_4
// refs: https://github.com/0kk470/pil4/blob/master/chapter27/chapter27.cpp

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void* allocator(void* ud, void* ptr, size_t osize, size_t nsize) {
	(void)ud; (void)osize;  /* not used */
	if (nsize == 0) 
	{
		free(ptr);
		return NULL;
	}
	else
	{
		int* limit = (int*)ud;
		if (limit != NULL && *limit < nsize)
		{
			printf("fail, out of memory limit!\n");
			return NULL;		// ? 需要 free 一下吗？
		}
		return realloc(ptr, nsize);
	}
}

void setlimit(lua_State* L, int* memsize)
{
	lua_setallocf(L, allocator, memsize);
}

void exercise_27_4()
{
	lua_State* L = luaL_newstate();
	int limit = 8;
	setlimit(L, &limit);
	printf("Push 1234567\n");
	lua_pushstring(L, "1234567");
	printf("Push 89\n");
	lua_pushstring(L, "89");
}

/*

Push 1234567
fail, out of memory limit!
fail, out of memory limit!
fail, out of memory limit!
fail, out of memory limit!
PANIC: unprotected error in call to Lua API (not enough memory)


*/


