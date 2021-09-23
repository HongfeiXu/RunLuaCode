#pragma once

// C ���� Lua ����
// Refs: https://www.zhihu.com/collection/739084170

/*
1. ͨ��lua_getglobal���API��ȡlua������������ѹ������ջ
2. ����в����Ļ���ͨ������lua_pushnumber��lua_pushstring������API��������ѹ������ջ
3. ����lua_call(L,n,r)�����ĺ�ӿڣ����е��ã���֪�������������n��������r������ֵ
4. ������ɺ�lua�Ὣ����ֵѹ��ջ��C/C++�Ƕ˾Ϳ���ͨ������lua_tonumber������API����ȡ��������ֵ

TODO: ����û�з���ֵ�ĺ���Ҫ��ε����أ�

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
	// �����ַ������� lua ȫ�ֺ��� LuaCode_MyAdd
	luaL_dostring(lua, "function LuaCode_MyAdd(x, y) return x + y end");
	// lua ջѹ������
	lua_getglobal(lua, "LuaCode_MyAdd");
	lua_pushinteger(lua, 100);
	lua_pushinteger(lua, 200);
	// C����lua�еĺ�����2������ֵ��1�����ز���
	lua_call(lua, 2, 1);
	cout << "lua function ret: " << lua_tointeger(lua, -1) << endl;
	lua_pop(lua, 1);
	lua_close(lua);
	cout << "---------------test c call lua--------------------" << endl;
}

// C ���ʺ��޸� Lua ����

void Test_C_Modify_Lua_Variable()
{

}





