#pragma once

// Lua ���� C����
// Refs: https://www.zhihu.com/collection/739084170

/*
1. ��ͨC����ת��lua_CFunction��ʽ
2. ת��lua_CFunction��ʽע�ᵽluaȫ�ֱ��У�������һ���������ƣ�һ�㱣����Cһ��������
3. ��lua�����е���ע����ĺ��������վͻ���õ���ͨC����
4. ���ù���ʵ�ʣ�lua�Ȱ�ע��ĺ���ѹ������ջ�У��ٽ�����ѹ�룬ִ��C������Ȼ�����ǵ���ջ��������ֵѹ�뵽����ջ��
*/


#include <iostream>
#include <string>
#include "LuaUtils.h"
using namespace std;


// ������������C++д�ģ���LuaLib��C�������ɵģ��������Ƕ���LuaLib���̵�ͷ�ļ���
// #includeʱҪ����extern "C",��֪����������ʱ��Ҫ��C++�������ҷ��ţ���һ������Ҫ��
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static int lua_CCode_MyAdd(lua_State* lua);		// forward declaration

static int lua_CCode_MyAddClosure(lua_State* L)
{
	lua_pushinteger(L, 9);	// ������ lua_CCode_MyAdd ��һ��upvalue
	lua_pushcclosure(L, &lua_CCode_MyAdd, 1);
	return 1;
}

static int lua_CCode_MyAdd(lua_State * lua)		// lua_CFunction ��ʽ
{
	lua_Integer upvalue = lua_tointeger(lua, lua_upvalueindex(1));
	printf("upvalue = %lld\n", upvalue);
	int n = lua_gettop(lua);	// �����յ��Ĳ�������
	if (n != 2)
	{
		luaL_error(lua, "params error");	// TODO: �ϲ����ﴦ�����error
	}
	lua_Integer n1 = lua_tointeger(lua, -1);		// �� lua ����ջ ȡ����������
	lua_Integer n2 = lua_tointeger(lua, -2);
	lua_Integer iRet = n1 + n2 + upvalue;
	lua_pushinteger(lua, iRet);					// �����ѹ�� ����ջ
	return 1;
}

static int lua_CCode_AverageSum(lua_State * L)
{
	int n = lua_gettop(L);
	if (n < 1)
	{
		luaL_error(L, "params error");	// TODO: �ϲ����ﴦ�����error
	}
	lua_Number sum = 0.0;
	int i;
	for (i = 1; i <= n; ++i)
	{
		// note: �������� luaL_checknumber �������ͼ��
		if (!lua_isnumber(L, i))
		{
			luaL_error(L, "incorrect argument");
		}
		sum += lua_tonumber(L, i);
	}
	lua_pushnumber(L, sum / n);		// ��1������ֵ
	lua_pushnumber(L, sum);			// ��2������ֵ
	return 2;
}

void Test_CCode_For_Lua()
{
	cout << "---------------test c code for lua--------------------" << endl;
	cout << "-------------test1--------------" << endl;
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_register(lua, "CCode_MyAddClosure", lua_CCode_MyAddClosure);		// �� pcf_CCode_MyAdd ע��� CCode_MyAdd �ӵ�ȫ�ֱ���
	luaL_dostring(lua, "addfunc = CCode_MyAddClosure()");
	int errcode = luaL_dostring(lua, "print(\"lua add:\"..addfunc(100,1000))");	// ���ַ���ת��Ϊlua�ļ�������ִ��
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

