#pragma once

#include "luna.h"

int func_a(const char* a, int b)
{
	return b;
}

class my_class final {
	int age;
	int money;

public:
	my_class()
		: age(10), money(10)
	{
	}
	int func_a()
	{
		return 999;
	}
	int func_b()
	{
		return 8888;
	}
public:
	DECLARE_LUA_CLASS(my_class);
};

LUA_EXPORT_CLASS_BEGIN(my_class)
LUA_EXPORT_METHOD(func_a)
LUA_EXPORT_PROPERTY(age)
LUA_EXPORT_CLASS_END()

// NOT WORKING!!!

//int get_obj_from_cpp(lua_State* L)
//{
//	lua_push_object(L, my_class());
//	return 1;
//}
