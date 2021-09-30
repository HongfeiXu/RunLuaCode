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

// ����luaԴ�ļ���������һ��
void load(lua_State* L, const char* fname)
{
	// luaL_loadfile �ײ��ߵ��� lua_load��
	// ���û�д��󣬽�����õĴ����ѹ��ջ��
	// ���򣬽�������Ϣѹ��ջ��
	// lua_pcall run the compiled chunk
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
	{
		simple_error_handler(L, "cannot run config. file: %s", lua_tostring(L, -1));
	}
}

int getglobint(lua_State* L, const char* var)
{
	int isnum, result;
	lua_getglobal(L, var);	// ��ȫ�ֱ��� var ���ֵѹջ�����ظ�ֵ������
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

// ��ȡ��ɫtable��ָ��key����ɫ
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
	// �ָ�ջ��״̬
	// ��ջ1��Ԫ�أ�����table�ж������Ǹ����֣�����ʱջ�л���һ��table
	lua_pop(L, 1);
	return result;
}

// ��ȡ��ɫtable
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

