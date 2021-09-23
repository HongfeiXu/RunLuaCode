#pragma once

// Lua 调用 C函数
// Refs: https://www.zhihu.com/collection/739084170

/*
1. 普通C函数转成lua_CFunction形式
2. 转成lua_CFunction形式注册到lua全局表中，并分配一个函数名称，一般保留和C一样的名称
3. 在lua代码中调用注册过的函数，最终就会调用到普通C函数
4. 调用过程实际：lua先把注册的函数压入虚拟栈中，再将参数压入，执行C函数，然后将它们弹出栈，将返回值压入到虚拟栈顶
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

extern "C" long long CCode_MyAdd(long long a, long long b)
{
	return a + b;
}

extern "C" int pcf_CCode_MyAdd(lua_State * lua)		// lua_CFunction 形式
{
	long long n1 = lua_tointeger(lua, -1);		// 从 lua 虚拟栈 取出两个整数
	long long n2 = lua_tointeger(lua, -2);
	long long iRet = CCode_MyAdd(n1, n2);
	lua_pushinteger(lua, iRet);					// 将结果压如 虚拟栈
	return 1;
}

void Test_CCode_For_Lua()
{
	cout << "---------------test c code for lua--------------------" << endl;
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_register(lua, "CCode_MyAdd", pcf_CCode_MyAdd);		// 将 pcf_CCode_MyAdd 注册成 CCode_MyAdd 加到全局表中
	luaL_dostring(lua, "print(\"lua add:\"..CCode_MyAdd(100,1000))");	// 将字符串转换为lua文件并解析执行

	lua_close(lua);
	cout << "---------------test c code for lua--------------------" << endl;
}

