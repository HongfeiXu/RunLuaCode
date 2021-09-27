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
using namespace std;


// ������������C++д�ģ���LuaLib��C�������ɵģ��������Ƕ���LuaLib���̵�ͷ�ļ���
// #includeʱҪ����extern "C",��֪����������ʱ��Ҫ��C++�������ҷ��ţ���һ������Ҫ��
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

extern "C" int pcf_CCode_MyAdd(lua_State * lua)		// lua_CFunction ��ʽ
{
	int n = lua_gettop(lua);	// �����յ��Ĳ�������
	if (n != 2)
	{
		luaL_error(lua, "params error");	// TODO: �ϲ����ﴦ�����error
	}
	long long n1 = lua_tointeger(lua, -1);		// �� lua ����ջ ȡ����������
	long long n2 = lua_tointeger(lua, -2);
	long long iRet = CCode_MyAdd(n1, n2);
	lua_pushinteger(lua, iRet);					// �����ѹ�� ����ջ
	return 1;
}

extern "C" int pfc_CCode_AverageSum(lua_State * L)
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
	lua_register(lua, "CCode_MyAdd", pcf_CCode_MyAdd);		// �� pcf_CCode_MyAdd ע��� CCode_MyAdd �ӵ�ȫ�ֱ���
	luaL_dostring(lua, "print(\"lua add:\"..CCode_MyAdd(100,1000))");	// ���ַ���ת��Ϊlua�ļ�������ִ��
	cout << "-------------test2--------------" << endl;
	lua_register(lua, "CCode_AverageSum", pfc_CCode_AverageSum);
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

