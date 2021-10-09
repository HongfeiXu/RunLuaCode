#pragma once

// Exercise 31.5: Based on the example for Boolean arrays, 
// implement a small C library for integer arrays.

#include <limits.h>
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

#define checkintegerarray(L, n) (IntegerArray*)luaL_checkudata(L, n, "LuaBook.integerarray")

typedef struct IntegerArray {
	int size;
	int values[1];
}IntegerArray;

static int newarray(lua_State* L)
{
	int n = (int)luaL_checkinteger(L, 1);
	luaL_argcheck(L, n >= 1, 1, "invalid size");
	size_t nbytes = sizeof(IntegerArray) + (n - 1) * sizeof(int);
	IntegerArray* a = (IntegerArray*)lua_newuserdata(L, nbytes);
	a->size = n;
	for (int i = 0; i < n; ++i)
	{
		a->values[i] = 0;	// initialize array
	}
	luaL_getmetatable(L, "LuaBook.integerarray");
	lua_setmetatable(L, -2);
	return 1;
}

static int setarray(lua_State* L)
{
	IntegerArray* a = checkintegerarray(L, 1);
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, index >= 0 && index < a->size, 2, "index out of range");
	int v = (int)luaL_checkinteger(L, 3);
	a->values[index] = v;
	return 0;
}

static int getarray(lua_State* L)
{
	IntegerArray* a = checkintegerarray(L, 1);
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, index >= 0 && index < a->size, 2, "index out of range");
	lua_pushinteger(L, a->values[index]);
	return 1;
}

static int getsize(lua_State* L)
{
	IntegerArray* a = checkintegerarray(L, 1);
	lua_pushinteger(L, a->size);
	return 1;
}

char buffer[128];
static int array2string(lua_State* L)
{
	IntegerArray* a = checkintegerarray(L, 1);
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	luaL_addstring(&b, "=== integerarray start ===\n");
	sprintf(buffer, "size: %d\n", a->size);
	luaL_addstring(&b, buffer);
	for (int i = 0; i < a->size; ++i)
	{
		sprintf(buffer, "[%d] = %d\n", i + 1, a->values[i]);
		luaL_addstring(&b, buffer);
	}
	luaL_addstring(&b, "=== integerarray end ===\n");
	luaL_pushresult(&b);
	return 1;
}

static const struct luaL_Reg arraylib_f[] = {
	{"new", newarray},
	{NULL, NULL}
};

static const struct luaL_Reg arraylib_m[] = {
	{"__newindex", setarray},
	{"__index", getarray},
	{"__len", getsize},
	{"__tostring", array2string},
	{NULL, NULL}
};

int luaopen_arraylib(lua_State* L)
{
	luaL_newmetatable(L, "LuaBook.integerarray");
	luaL_setfuncs(L, arraylib_m, 0);
	luaL_newlib(L, arraylib_f);
	return 1;
}

void require_arraylib(lua_State* L)
{
	luaL_requiref(L, "integerarray", luaopen_arraylib, 1);
	lua_pop(L, 1);
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_arraylib(L);
	if (luaL_dofile(L, "lua/IntegerArray.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

