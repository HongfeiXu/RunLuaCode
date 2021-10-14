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

#define checkarray(L, n) (BitArray*)luaL_checkudata(L, n, "LuaBook.booleanarray")
#define checkarrayindex(bitarray, i) (bitarray->values[(I_WORD(i))] & I_BIT(i))

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

	luaL_getmetatable(L, "LuaBook.booleanarray");
	lua_setmetatable(L, -2);	// 将 LuaBook.booleanarray 设为 userdata 的 metatable，
								// 这样所有的 array 都关联了这个 metatable
								// 基于此，做类型合法性的判断
	return 1;
}

static unsigned int* getparams(lua_State* L, unsigned int* mask)
{
	BitArray* a = checkarray(L, 1);
	int index = (int)luaL_checkinteger(L, 2) - 1;
	luaL_argcheck(L, 0 <= index && index < a->size, 2, "index out of range");

	*mask = I_BIT(index);	// mask to access correct bit
	return &a->values[I_WORD(index)];	// word address
}

static int setarray(lua_State* L)
{
	unsigned int mask;
	unsigned int* entry = getparams(L, &mask);
	//luaL_checkany(L, 3);
	luaL_checktype(L, 3, LUA_TBOOLEAN);	// Exercise 31.1
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
	BitArray* a = checkarray(L, 1);
	lua_pushinteger(L, a->size);
	return 1;
}

// Exercise 31.4: Modify the implementation of the __tostring metamethod so that it shows the full con -
// tents of the array in an appropriate way.Use the buffer facility(the section called “String Manipulation”)
// to create the resulting string.
static int array2string(lua_State* L)
{
	BitArray* a = checkarray(L, 1);

	luaL_Buffer b;
	char* p = luaL_buffinitsize(L, &b, a->size);
	for (int i = 0; i < a->size; ++i)
	{
		if (checkarrayindex(a, i))
			p[i] = '1';
		else
			p[i] = '0';
	}
	luaL_pushresultsize(&b, a->size);
	return 1;
}

char buffer[128];
static int array2string_elegant(lua_State* L)	// 更优雅的输出
{
	BitArray* a = checkarray(L, 1);

	luaL_Buffer b;
	luaL_buffinit(L, &b);
	luaL_addstring(&b, "==== booleanarray start ===\n");
	sprintf(buffer, "size: %d\n", a->size);
	luaL_addstring(&b, buffer);
	for (int i = 0; i < a->size; ++i)
	{
		sprintf(buffer, "[%d] = %s\n", i + 1, checkarrayindex(a, i) ? "true" : "false");
		luaL_addstring(&b, buffer);
	}
	luaL_addstring(&b, "==== booleanarray end ===\n");
	luaL_pushresult(&b);
	return 1;
}

// Exercise 31.3: Extend the previous exercise so that we can use addition to get the union of two arrays and
// multiplication for the intersection.

static int getintersection(lua_State* L)
{
	BitArray* lhs = checkarray(L, 1);
	BitArray* rhs = checkarray(L, 2);
	int len = __min(lhs->size, rhs->size);
	lua_pushcfunction(L, newarray);
	lua_pushinteger(L, len);
	if (lua_pcall(L, 1, 1, 0) == LUA_OK)
	{
		BitArray* ret = (BitArray*)lua_touserdata(L, -1);
		for (int i = 0; i < len; ++i)
		{
			if (checkarrayindex(lhs, i) && checkarrayindex(rhs, i))
			{
				ret->values[I_WORD(i)] != I_BIT(i);
			}
			else
			{
				ret->values[I_WORD(i)] &= ~I_BIT(i);
			}
		}
	}
	else
	{
		simple_error_handler(L, "create new array failed:", lua_tostring(L, -1));
	}
	return 1;
}

static int getunion(lua_State* L)
{
	BitArray* lhs = checkarray(L, 1);
	BitArray* rhs = checkarray(L, 2);
	int len = __max(lhs->size, rhs->size);
	lua_pushcfunction(L, newarray);
	lua_pushinteger(L, len);
	if (lua_pcall(L, 1, 1, 0) == LUA_OK)
	{
		BitArray* tmp = lhs->size < rhs->size ? lhs : rhs;
		BitArray* ret = (BitArray*)lua_touserdata(L, -1);
		int i = 0;
		// 先用公共部分按位取或
		for (i = 0; i < tmp->size; ++i)
		{
			if (checkarrayindex(lhs, i) || checkarrayindex(rhs, i))
			{
				ret->values[I_WORD(i)] |= I_BIT(i);
			}
			else
			{
				ret->values[I_WORD(i)] &= ~I_BIT(i);
			}
		}
		// 余下内容用长的那个补齐
		tmp = lhs->size < rhs->size ? rhs : lhs;
		for (; i < len; ++i)
		{
			if (checkarrayindex(tmp, i))
			{
				ret->values[I_WORD(i)] |= I_BIT(i);
			}
			else
			{
				ret->values[I_WORD(i)] &= ~I_BIT(i);
			}
		}
	}
	else
	{
		simple_error_handler(L, "create new array failed:", lua_tostring(L, -1));
	}
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
	{"__newindex", setarray},	// other operations come only as methods
	{"__index", getarray},
	{"__len", getsize},
	{"__tostring", array2string_elegant},
	{"__add", getunion},
	{"__mul", getintersection},
	{NULL, NULL}
};

int luaopen_arraylib(lua_State* L)
{
	luaL_newmetatable(L, "LuaBook.booleanarray");	// create the metatable for array
	lua_pushvalue(L, -1);
	lua_pushvalue(L, -1);				// duplicate the metatable
	lua_setfield(L, -2, "__index");		// mt.__index = mt
	luaL_setfuncs(L, arraylib_m, 0);	// register metamethods
	luaL_newlib(L, arraylib_f);			// create lib table, register functions
	return 1;
}

void require_arraylib(lua_State* L)
{
	// https://www.lua.org/manual/5.4/manual.html#luaL_requiref
	luaL_requiref(L, "booleanarray", luaopen_arraylib, 1);
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

