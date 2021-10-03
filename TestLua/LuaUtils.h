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
void simple_error_handler(lua_State* L, const char* fmt, ...);

// 打印栈中指定的元素
void print_stack_value(lua_State* L, int i);

// Deumping the stack
static void stack_dump(lua_State* L)
{
	int i;
	int top = lua_gettop(L);	// depth of the stack
	for (i = 1; i <= top; ++i)
	{
		print_stack_value(L, i);
		printf("   ");
	}
	printf("\n");
}

// 加载lua源文件
void load_lua_src(lua_State* L, const char* fname);

// table traversal
void traverse_table(lua_State* L, int index);
