#pragma once

// C 调用 Lua 函数
// Refs: https://www.zhihu.com/collection/739084170

/*
1. 通过lua_getglobal这个API获取lua函数，并将其压入虚拟栈
2. 如果有参数的话，通过类似lua_pushnumber，lua_pushstring这样的API，将参数压入虚拟栈
3. 调用lua_call(L,n,r)这样的宏接口，进行调用，告知虚拟机，函数有n个参数，r个返回值
4. 调用完成后，lua会将返回值压入栈，C/C++那端就可以通过类似lua_tonumber这样的API来读取函数返回值
*/

#include <iostream>
#include <string>
using namespace std;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void Test_C_Call_LuaCode()
{
	cout << "---------------test c call lua--------------------" << endl;
	lua_State* lua = luaL_newstate();
	// 加载字符，生成 lua 全局函数 LuaCode_MyAdd
	luaL_dostring(lua, "function LuaCode_MyAdd(x, y) return x + y end");
	// lua 栈压入数据
	lua_getglobal(lua, "LuaCode_MyAdd");
	lua_pushinteger(lua, 100);
	lua_pushinteger(lua, 200);
	// C调用lua中的函数，2个传入值，1个返回参数
	lua_call(lua, 2, 1);
	cout << "lua function ret: " << lua_tointeger(lua, -1) << endl;
	lua_pop(lua, 1);
	lua_close(lua);
	cout << "---------------test c call lua--------------------" << endl;
}

/*

---------------test c call lua--------------------
lua function ret: 300
---------------test c call lua--------------------

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 18748) exited with code 0.
Press any key to close this window . . .

*/

// C 访问和修改 Lua 变量
void Test_C_Modify_Lua_Variable()
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);		// TODO: 这个玩意是干嘛的？？？不加这个， ShowTable 就跑不了
	// C 访问 Lua 文件
	// luaL_dofile 成功的话，返回值是0，非0则表示有错误，这里输出错误信息
	if (luaL_dofile(lua, "test.lua"))	
	{
		cout << lua_tostring(lua, -1) << endl;
	}

	// Lua 中的函数
	lua_getglobal(lua, "LuaFileAdd");
	lua_pushnumber(lua, 12.3);
	lua_pushnumber(lua, 23.4);
	lua_call(lua, 2, 1);
	cout << "lua file function ret: " << lua_tonumber(lua, -1) << endl;

	// Lua 中的数据
	// TODO: 这里的栈操作不是很清楚，学一哈
	lua_getglobal(lua, "LuaFileStr");
	cout << "lua file str: " << lua_tostring(lua, -1) << endl;
	lua_getglobal(lua, "LuaFileTable");
	lua_getfield(lua, -1, "age");
	cout << "lua file table age: " << lua_tonumber(lua, -1) << endl;
	lua_getfield(lua, -2, "name");
	cout << "lua file table name: " << lua_tostring(lua, -1) << endl;
	lua_getglobal(lua, "ShowTable");
	lua_call(lua, 0, 0);


	// 修改
	lua_pushstring(lua, "Lilei");
	lua_setfield(lua, 3, "name");		// ??? 这个 3 是如何计算出来的
	lua_getfield(lua, 3, "name");
	cout << "lua file new t able name: " << lua_tostring(lua, -1) << endl;
	lua_getglobal(lua, "ShowTable");
	lua_call(lua, 0, 0);
}

/*

lua file function ret: 35.7
lua file str: hello lua string
lua file table age: 27
lua file table name: Hongfei
Name:Hongfei
Age:27
lua file new t able name: Lilei
Name:Lilei
Age:27

*/





