#pragma once

// C 调用 Lua 函数
// Refs: https://www.zhihu.com/collection/739084170

/*
1. 通过lua_getglobal这个API获取lua函数，并将其压入虚拟栈
2. 如果有参数的话，通过类似lua_pushnumber，lua_pushstring这样的API，将参数压入虚拟栈
3. 调用lua_call(L,n,r)这样的宏接口，进行调用，告知虚拟机，函数有n个参数，r个返回值
4. 调用完成后，lua会将返回值压入栈，C/C++那端就可以通过类似lua_tonumber这样的API来读取函数返回值

TODO: 对于没有返回值的函数要如何调用呢？

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
	luaL_openlibs(lua);
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

// C 访问和修改 Lua 变量

void Test_C_Modify_Lua_Variable()
{

}





