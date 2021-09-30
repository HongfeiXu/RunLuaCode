#pragma once

// Refs: Programming in Lua, 4th ed. Ch28 Extending Your Application
// Getting user information from a configuration file

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "SimpleLuaInterpreter.h"
#include "StackDump.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// 加载lua源文件，并运行一下
void load(lua_State* L, const char* fname)
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

int getglobint(lua_State* L, const char* var)
{
	int isnum, result;
	lua_getglobal(L, var);	// 把全局变量 var 里的值压栈，返回该值的类型
	result = (int)lua_tointegerx(L, -1, &isnum);	// convert this value to an integer, ensuring that it has the correct type
	if (!isnum)
	{
		simple_error_handler(L, "'%s' should be a number\n", var);
	}
	lua_pop(L, 1);	// remove result from the stack
	return result;
}

void load_test()
{
	lua_State* L = luaL_newstate();
	int w, h;
	load(L, "config_data.lua");
	w = getglobint(L, "width");
	h = getglobint(L, "height");
	printf("w = %d, h = %d\n", w, h);
	lua_close(L);
}

/*

w = 200, h = 300

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 38048) exited with code 0.
Press any key to close this window . . .

*/

#define MAX_COLOR 255

// 获取颜色table中指定key的颜色
// assume that table is on the top of the stack
int get_color_field(lua_State* L, const char* key)
{
	int isnum;
	double result;
	lua_getfield(L, -1, key);
	result = (int)(lua_tonumberx(L, -1, &isnum) * MAX_COLOR);
	if (!isnum)
	{
		simple_error_handler(L, "invalid component '%s' in color", key);
	}
	// 恢复栈的状态
	// 出栈1个元素（弹出table中读出的那个数字），此时栈中还有一个table
	lua_pop(L, 1);
	return result;
}

// 获取颜色table
int* get_glob_color(lua_State* L, const char* var)
{
	int* color = new int[3];
	lua_getglobal(L, var);
	if (!lua_istable(L, -1))
	{
		simple_error_handler(L, "'%s' is not a table", var);
	}
	color[0] = get_color_field(L, "red");
	color[1] = get_color_field(L, "green");
	color[2] = get_color_field(L, "blue");
	return color;
}

void get_glob_color_test()
{
	lua_State* L = luaL_newstate();
	load(L, "config_data.lua");
	const char* color_name = "background";
	int* color = get_glob_color(L, color_name);
	printf("%s = (%d, %d, %d)", color_name, color[0], color[1], color[2]);
	lua_close(L);
}

/*

background = (76, 25, 0)
G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 89068) exited with code 0.
Press any key to close this window . . .

*/

