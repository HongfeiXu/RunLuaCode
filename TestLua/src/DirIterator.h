#pragma once

// Refs: Programming in Lua, 4th ed. Ch32 Managing Resources


#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

static int dir_iter(lua_State* L);

// a factory function that Lua calls to create iterators;
// it must open a DIR structure and create a closure of 
// the iterator function with this structure as an upvalue
// 用 userdata（保存DIR**） 作为 upvalue
static int l_dir(lua_State* L)
{
	const char* path = luaL_checkstring(L, 1);

	// create a userdata to store a DIR address
	DIR** d = (DIR**)lua_newuserdata(L, sizeof(DIR*));
	// pre-initialize it
	*d = NULL;

	// set its metatable
	luaL_getmetatable(L, "LuaBook.dir");
	lua_setmetatable(L, -2);

	// try to open the given directory
	*d = opendir(path);	// DIR once created, is immediately associated with the userdata
	if (*d == NULL)
	{
		luaL_error(L, "cannot open %s: %s", path, strerror(errno));
	}
	// create and returns the iterator function;
	// its sole upvalue, the directory userdata,
	// is already on the top of the stack
	lua_pushcclosure(L, dir_iter, 1);
	return 1;
}

static int dir_iter(lua_State* L)
{
	DIR* d = *(DIR**)lua_touserdata(L, lua_upvalueindex(1));	// 获取 upvalue DIR**
	struct dirent* entry = readdir(d);
	if (entry != NULL)
	{
		lua_pushstring(L, entry->d_name);
		return 1;
	}
	// no more values to return
	else
	{
		return 0;
	}
}

static int dir_gc(lua_State* L)
{
	stack_dump(L);
	DIR* d = *(DIR**)lua_touserdata(L, 1);	// ??? 这里为啥userdata直接在栈中了？？
	if (d) closedir(d);
	return 0;
}

static const struct luaL_Reg dirlib[] = {
	{"open", l_dir},	// 只暴露这个函数到lua层
	{NULL, NULL}
};

int luaopen_dirlib(lua_State* L)
{
	luaL_newmetatable(L, "LuaBook.dir");
	// set its __gc field
	// 设置 metatable 的 __gc
	lua_pushcfunction(L, dir_gc);
	lua_setfield(L, -2, "__gc");

	// create the library
	luaL_newlib(L, dirlib);
	return 1;
}

void require_dirlib(lua_State* L)
{
	luaL_requiref(L, "dir", luaopen_dirlib, 1);
	lua_pop(L, 1);
}

void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_dirlib(L);
	if (luaL_dofile(L, "lua/dir.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}

