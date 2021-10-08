#pragma once

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

// Manipulating a Boolean array

/*

a = array.new(1000)
for i = 1, 1000 do
	array.set(a, i, i % 2 == 0) -- a[i] = (i % 2 == 0)
end
print(array.get(a, 10)) --> true
print(array.get(a, 11)) --> false
print(array.size(a)) --> 1000

*/

#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int)) // the number of bits in an unsigned integer
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD) // 需要多少个 unsigned int 去存 0、1 值，BitArray中已经有values[0]，这里不需要向上取整
#define I_BIT(i) (1 << ((unsigned int)(i) % BITS_PER_WORD)) // computes a mask to access the correct bit inside this word

#define checkarray(L) (BitArray*)lua_touserdata(L, 1)

typedef struct BitArray {
	int size;
	unsigned int values[1];
}BitArray;

static int newarray(lua_State* L)
{
	int i;
	size_t nbytes;
	BitArray* a;

	int n = (int)luaL_checkinteger(L, 1);	// number of bits
	luaL_argcheck(L, n >= 1, 1, "invalid size");
	nbytes = sizeof(BitArray) + I_WORD(n - 1) * sizeof(unsigned int);
	a = (BitArray*)lua_newuserdata(L, nbytes);
	
	a->size = n;
	for (i = 0; i <= I_WORD(n - 1); ++i)
	{
		a->values[i] = 0;	// initialize array
	}

	luaL_getmetatable(L, "LuaBook.array");
	lua_setmetatable(L, -2);	// 将 LuaBook.array 设为 userdata 的 metatable，
								// 这样所有的 array 都关联了这个 metatable
								// 基于此，做类型合法性的判断

	return 1;
}

static unsigned int* getparams(lua_State* L, unsigned int* mask)
{
	BitArray* a = checkarray(L);
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");

	*mask = I_BIT(index);	// mask to access correct bit
	return &a->values[I_WORD(index)];	// word address
}

static int setarray(lua_State* L)
{
	unsigned int mask;
	unsigned int* entry = getparams(L, &mask);
	luaL_checkany(L, 3);
	if (lua_toboolean(L, 3))
	{
		*entry |= mask;	// set bit
	}
	else
	{
		*entry &= ~mask;	// reset bit
	}
	return 0;
}

static int getarray(lua_State* L)
{
	unsigned int mask;
	unsigned int* entry = getparams(L, &mask);

	lua_pushboolean(L, *entry & mask);
	return 1;
}

static int getsize(lua_State* L)
{
	BitArray* a = checkarray(L);
	lua_pushinteger(L, a->size);
	return 1;
}

static int array2string(lua_State* L)
{
	BitArray* a = checkarray(L);
	lua_pushfstring(L, "array(%d)", a->size);
	return 1;
}

// Make a C Module

// function
static const struct luaL_Reg arraylib_f[] = {
	{"new", newarray},	// the only function that our library has to export
	{NULL, NULL}
};

// method
static const struct luaL_Reg arraylib_m[] = {
	{"set", setarray},	// other operations come only as methods
	{"get", getarray},
	{"size", getsize},
	{"__tostring", array2string},
	{NULL, NULL}
};

int luaopen_arraylib(lua_State* L)
{
	luaL_newmetatable(L, "LuaBook.array");	// create the metatable for array
	lua_pushvalue(L, -1);				// duplicate the metatable
	lua_setfield(L, -2, "__index");		// mt.__index = mt
	luaL_setfuncs(L, arraylib_m, 0);	// register metamethods
	luaL_newlib(L, arraylib_f);			// create lib table, register functions
	return 1;
}

void require_arraylib(lua_State* L)
{
	// https://www.lua.org/manual/5.4/manual.html#luaL_requiref
	luaL_requiref(L, "array", luaopen_arraylib, 1);
	lua_pop(L, 1);
}

// test

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_arraylib(L);
	if (luaL_dofile(L, "lua/BooleanArray.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

/*

1000
true
false
array(1000)

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 65940) exited with code 0.
Press any key to close this window . . .

*/

