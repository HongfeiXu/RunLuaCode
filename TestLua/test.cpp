#define USE_BOOK_CODE 1		// 是否编译 Programming in Lua 中的代码

#if !USE_BOOK_CODE


#include "LuaCallCFunc.h"
#include "CCallLuaFunc.h"

int main()
{
	Test_CCode_For_Lua();			// Lua 调用 C 函数
	Test_C_Call_LuaCode();			// C 调用 Lua 函数
	Test_C_Modify_Lua_Variable();	// C 修改 Lua 变量

	return 0;
}

#else


#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void simple_error_handler(lua_State* L, const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

int main(void)
{
	char buff[256];
	int error;
	lua_State* L = luaL_newstate();	// opens Lua
	luaL_openlibs(L);				// opens the standard libraries
	while (fgets(buff, sizeof(buff), stdin) != NULL)
	{
		error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
		if (error)
		{
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);			// pop error message from the stack
		}
	}
	lua_close(L);
	return 0;
}


#endif


