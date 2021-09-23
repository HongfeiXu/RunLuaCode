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
	long long n1 = lua_tointeger(lua, -1);		// �� lua ����ջ ȡ����������
	long long n2 = lua_tointeger(lua, -2);
	long long iRet = CCode_MyAdd(n1, n2);
	lua_pushinteger(lua, iRet);					// �����ѹ�� ����ջ
	return 1;
}

void Test_CCode_For_Lua()
{
	cout << "---------------test c code for lua--------------------" << endl;
	lua_State* lua = luaL_newstate();
	luaL_openlibs(lua);
	lua_register(lua, "CCode_MyAdd", pcf_CCode_MyAdd);		// �� pcf_CCode_MyAdd ע��� CCode_MyAdd �ӵ�ȫ�ֱ���
	luaL_dostring(lua, "print(\"lua add:\"..CCode_MyAdd(100,1000))");	// ���ַ���ת��Ϊlua�ļ�������ִ��

	lua_close(lua);
	cout << "---------------test c code for lua--------------------" << endl;
}

