#pragma once

// Refs: Programming in Lua, 4th ed. Ch28 Extending Your Application

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

// --------------------------------------------------------
// Getting user information from a configuration file
// --------------------------------------------------------

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
	load_lua_src(L, "lua/config_data.lua");
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


// --------------------------------------------------------
// A praticular get_color_field implementation
// --------------------------------------------------------

#define MAX_COLOR 255

// 获取颜色table中指定key的颜色
// assume that table is on the top of the stack
int get_color_field(lua_State* L, const char* key)
{
	int result;
	// lua_getfield: 把 t[key] 的值压入栈，这里的t时索引指向的值。返回压入值的类型
	if (lua_getfield(L, -1, key) != LUA_TNUMBER)
	{
		simple_error_handler(L, "invalid component '%s' in color", key);
	}
	result = (int)(lua_tonumber(L, -1) * MAX_COLOR);
	// 恢复栈的状态
	// 出栈1个元素（弹出table中读出的那个数字），此时栈中还有一个table
	lua_pop(L, 1);
	return result;
}

// 获取颜色table
int* get_glob_color(lua_State* L, const char* var)
{
	int* color = new int[3];
	lua_getglobal(L, var);		// 把全局变量 var 里的值压栈，返回该值的类型
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
	load_lua_src(L, "lua/config_data.lua");
	const char* color_name = "BACKGROUND";
	int* color = get_glob_color(L, color_name);
	printf("%s = (%d, %d, %d)\n", color_name, color[0], color[1], color[2]);
	lua_close(L);
}

/*

background = (76, 25, 0)
G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 89068) exited with code 0.
Press any key to close this window . . .

*/


// --------------------------------------------------------
// Colors as strings or tables
// --------------------------------------------------------


struct ColorTable {
	const char* name;
	unsigned char red, green, blue;
} colortable[] = {
	{"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
	{"RED", MAX_COLOR, 0, 0},
	{"GREEN", 0, MAX_COLOR, 0},
	{"CYAN", MAX_COLOR, MAX_COLOR},

	{NULL, 0, 0, 0}		// sentinel
};

// assume that table is on top
void set_color_field(lua_State* L, const char* index, int value)
{
	//lua_pushstring(L, index);						// 将指针 index 指向的零结尾的字符串压栈
	//lua_pushnumber(L, (double)value / MAX_COLOR);	// 把一个值为 n 的浮点数压栈
	//// 做一个等价于 t[k] = v 的操作， 这里 t 是给出的索引处的值(-3)， v 是栈顶的那个值， k 是栈顶之下的值。
	//// 这个函数会将键和值都弹出栈。
	//lua_settable(L, -3);

	// 等价于上面3句
	lua_pushnumber(L, (double)value / MAX_COLOR);	// 把一个值为 n 的浮点数压栈
	lua_setfield(L, -2, index);		// 做一个等价于 t[k] = v 的操作， 这里 t 是给出的索引处的值， 而 v 是栈顶的那个值。
									// 这个函数将把这个值弹出栈。
}

// defines a single color
// It creates a table, sets the appropiate fields and assigns this table to the corresponding global variable
void set_color(lua_State* L, struct ColorTable* ct)
{
	//lua_newtable(L);	// creates an empty table and pushes it on the stack
	lua_createtable(L, 0, 3);
	set_color_field(L, "red", ct->red);
	set_color_field(L, "green", ct->green);
	set_color_field(L, "blue", ct->blue);
	lua_setglobal(L, ct->name);		// 从堆栈上弹出一个值，并将其设为全局变量 name 的新值。
}

void set_color_test()
{
	lua_State* L = luaL_newstate();
	
	// 1. 将 colortable 中的数据写到 lua 全局环境中
	int i = 0;
	while (colortable[i].name != NULL)
	{
		set_color(L, &colortable[i++]);
	}
	// 2. 读取全局变量中的对应颜色table，获取颜色值并打印
	const char* color_name = "CYAN";
	int* color = get_glob_color(L, color_name);
	printf("%s = (%d, %d, %d)\n", color_name, color[0], color[1], color[2]);

	lua_close(L);
}

// Color as strings or tables
void colors_as_strings_or_tables(lua_State* L, const char* color_name)
{
	unsigned char red, green, blue;
	lua_getglobal(L, color_name);		// 把全局变量 var 里的值压栈，返回该值的类型
	if (lua_isstring(L, -1))			// value is a string?
	{
		const char* name = lua_tostring(L, -1);
		int i;	// search the colortable
		for (i = 0; colortable[i].name != NULL; i++)
		{
			if (strcmp(name, colortable[i].name) == 0)
			{
				break;
			}
		}
		if (colortable[i].name == NULL)
		{
			simple_error_handler(L, "invalid color name (%s)", color_name);
		}
		else 
		{
			red = colortable[i].red;
			green = colortable[i].green;
			blue = colortable[i].blue;
		}
	}
	else if (lua_istable(L, -1))		// value is table
	{
		red = get_color_field(L, "red");	// get color from table
		green = get_color_field(L, "green");
		blue = get_color_field(L, "blue");
	}
	else
	{
		simple_error_handler(L, "invalid value for %s", color_name);
	}
	printf("%s = (%d, %d, %d)\n", color_name, red, green, blue);
}

void color_as_strings_or_tables_test()
{
	lua_State* L = luaL_newstate();
	
	// 1. 将 colortable 中的数据写到 lua 全局环境中
	int i = 0;
	while (colortable[i].name != NULL)
	{
		set_color(L, &colortable[i++]);
	}

	// 2. 读取配置文件中的颜色
	load_lua_src(L, "lua/config_data.lua");

	colors_as_strings_or_tables(L, "FOREGROUND");		// from string
	colors_as_strings_or_tables(L, "CYAN");				// from table
	lua_close(L);
}

/*

FOREGROUND = (0, 255, 0)
CYAN = (255, 255, 0)

*/
