#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lunar.h"

class Account {
	lua_Number m_balance;
public:
	static const char className[];
	static Lunar<Account>::RegType methods[];

	Account(lua_State* L) { m_balance = luaL_checknumber(L, 1); }
	int deposit(lua_State* L) { m_balance += luaL_checknumber(L, 1); return 0; }
	int withdraw(lua_State* L) { m_balance -= luaL_checknumber(L, 1); return 0; }
	int balance(lua_State* L) { lua_pushnumber(L, m_balance); return 1; }
	~Account() { printf("deleted Account (%p)\n", this); }
};

const char Account::className[] = "Account";

Lunar<Account>::RegType Account::methods[] = {
  LUNAR_DECLARE_METHOD(Account, deposit),
  LUNAR_DECLARE_METHOD(Account, withdraw),
  LUNAR_DECLARE_METHOD(Account, balance),
  {0,0}
};

void test()
{
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);

	Lunar<Account>::Register(L);

	if (luaL_dofile(L, "lua/lunar_test.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}

	lua_gc(L, LUA_GCCOLLECT, 0);  // collected garbage
	lua_close(L);
}