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
#include "LuaUtils.h"
using namespace std;


// 本工程由于是C++写的，但LuaLib是C工程生成的，所以我们对于LuaLib工程的头文件，
// #include时要加上extern "C",告知编译器链接时不要按C++方法查找符号，这一步很重要。
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int lua_CCode_MyAdd(lua_State* lua);		// forward declaration

static int lua_CCode_MyAddClosure(lua_State* L)
{
	lua_pushinteger(L, 9);	// 给函数 lua_CCode_MyAdd 加一个upvalue
	lua_pushcclosure(L, &lua_CCode_MyAdd, 1);
	return 1;
}

static int lua_CCode_MyAdd(lua_State * lua)		// lua_CFunction 形式
{
	lua_Integer upvalue = lua_tointeger(lua, lua_upvalueindex(1));
	printf("upvalue = %lld\n", upvalue);
	int n = lua_gettop(lua);	// 函数收到的参数个数
	if (n != 2)
	{
		luaL_error(lua, "params error");	// TODO: 上层哪里处理这个error
	}
	lua_Integer n1 = lua_tointeger(lua, -1);		// 从 lua 虚拟栈 取出两个整数
	lua_Integer n2 = lua_tointeger(lua, -2);
	lua_Integer iRet = n1 + n2 + upvalue;
	lua_pushinteger(lua, iRet);					// 将结果压如 虚拟栈
	return 1;
}

static int lua_CCode_AverageSum(lua_State * L)
{
	int n = lua_gettop(L);
	if (n < 1)
	{
		luaL_error(L, "params error");	// TODO: 上层哪里处理这个error
	}
	lua_Number sum = 0.0;
	int i;
	for (i = 1; i <= n; ++i)
	{
		// note: 这块可以用 luaL_checknumber 来做类型检测
		if (!lua_isnumber(L, i))
		{
			luaL_error(L, "incorrect argument");
		}
		sum += lua_tonumber(L, i);
	}
	lua_pushnumber(L, sum / n);		// 第1个返回值
	lua_pushnumber(L, sum);			// 第2个返回值
	return 2;
}

void Test_CCode_For_Lua()
{
	cout << "---------------test c code for lua--------------------" << endl;
	cout << "-------------test1--------------" << endl;
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_register(lua, "CCode_MyAddClosure", lua_CCode_MyAddClosure);		// 将 pcf_CCode_MyAdd 注册成 CCode_MyAdd 加到全局表中
	luaL_dostring(lua, "addfunc = CCode_MyAddClosure()");
	int errcode = luaL_dostring(lua, "print(\"lua add:\"..addfunc(100,1000))");	// 将字符串转换为lua文件并解析执行
	if (errcode != LUA_OK)
	{
		simple_error_handler(lua, "%s\n", lua_tostring(lua , -1));
	}

	cout << "-------------test2--------------" << endl;
	lua_register(lua, "CCode_AverageSum", lua_CCode_AverageSum);
	luaL_dostring(lua, "print(CCode_AverageSum(1,2,3,4,5))");
	lua_close(lua);
	cout << "---------------test c code for lua--------------------" << endl;
}

/*

---------------test c code for lua--------------------
lua add:1100
---------------test c code for lua--------------------

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 57336) exited with code 0.
Press any key to close this window . . .

*/

