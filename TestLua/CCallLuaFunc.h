#pragma once

// C ���� Lua ����
// Refs: https://www.zhihu.com/collection/739084170

/*
1. ͨ��lua_getglobal���API��ȡlua������������ѹ������ջ
2. ����в����Ļ���ͨ������lua_pushnumber��lua_pushstring������API��������ѹ������ջ
3. ����lua_call(L,n,r)�����ĺ�ӿڣ����е��ã���֪�������������n��������r������ֵ
4. ������ɺ�lua�Ὣ����ֵѹ��ջ��C/C++�Ƕ˾Ϳ���ͨ������lua_tonumber������API����ȡ��������ֵ
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

/*

---------------test c call lua--------------------
lua function ret: 300
---------------test c call lua--------------------

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 18748) exited with code 0.
Press any key to close this window . . .

*/

// C ���ʺ��޸� Lua ����
void Test_C_Modify_Lua_Variable()
{
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);		// TODO: ��������Ǹ���ģ�������������� ShowTable ���ܲ���
	// C ���� Lua �ļ�
	// luaL_dofile �ɹ��Ļ�������ֵ��0����0���ʾ�д����������������Ϣ
	if (luaL_dofile(lua, "test.lua"))	
	{
		cout << lua_tostring(lua, -1) << endl;
	}

	// Lua �еĺ���
	lua_getglobal(lua, "LuaFileAdd");
	lua_pushnumber(lua, 12.3);
	lua_pushnumber(lua, 23.4);
	lua_call(lua, 2, 1);
	cout << "lua file function ret: " << lua_tonumber(lua, -1) << endl;

	// Lua �е�����
	// TODO: �����ջ�������Ǻ������ѧһ��
	lua_getglobal(lua, "LuaFileStr");
	cout << "lua file str: " << lua_tostring(lua, -1) << endl;
	lua_getglobal(lua, "LuaFileTable");
	lua_getfield(lua, -1, "age");
	cout << "lua file table age: " << lua_tonumber(lua, -1) << endl;
	lua_getfield(lua, -2, "name");
	cout << "lua file table name: " << lua_tostring(lua, -1) << endl;
	lua_getglobal(lua, "ShowTable");
	lua_call(lua, 0, 0);


	// �޸�
	lua_pushstring(lua, "Lilei");
	lua_setfield(lua, 3, "name");		// ??? ��� 3 ����μ��������
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





