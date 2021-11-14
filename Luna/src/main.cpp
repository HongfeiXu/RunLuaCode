#include <stdio.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "luna.h"
#include "luna_test.h"



int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_register_function(L, "func_a", func_a);
	//lua_pushcfunction(L, get_obj_from_cpp);	// push a value of type function
	//lua_setglobal(L, "get_obj_from_cpp");

	if (luaL_dofile(L, "src/luna_test.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}

	lua_close(L);
	return 0;
}
