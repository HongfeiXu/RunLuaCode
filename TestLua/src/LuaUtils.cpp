#include "LuaUtils.h"



void simple_error_handler(lua_State* L, const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void print_stack_value(lua_State* L, int i)
{
	int t = lua_type(L, i);
	switch (t)
	{
	case LUA_TSTRING:
	{
		printf("'%s'", lua_tostring(L, i));
		break;
	}
	case LUA_TBOOLEAN:
	{
		printf(lua_toboolean(L, i) ? "true" : "false");
		break;
	}
	case LUA_TNUMBER:
	{
		// integer?
		if (lua_isinteger(L, i))
		{
			printf("%lld", lua_tointeger(L, i));
		}
		// float
		else
		{
			printf("%g", lua_tonumber(L, i));
		}
		break;
	}
	default:
		printf("%s", lua_typename(L, t));
		break;
	}
}

void stack_dump(lua_State* L)
{
	printf("--stack_dump begin--\n");
	int i;
	int top = lua_gettop(L);	// depth of the stack
	for (i = 1; i <= top; ++i)
	{
		print_stack_value(L, i);
		printf("   ");
	}
	printf("\n");
	printf("--stack_dump end--\n");
}

void load_lua_src(lua_State* L, const char* fname)
{
	// luaL_loadfile 底层走的是 lua_load，
	// 如果没有错误，将编译好的代码块压到栈顶
	// 否则，将错误消息压到栈顶
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
	{
		simple_error_handler(L, "cannot run config. file: %s", lua_tostring(L, -1));
	}
}

void traverse_table(lua_State* L, int index)
{
	if (!lua_istable(L, index))
		return;
	lua_pushnil(L);
	if (index > 0)
	{
		index = index;
	}
	// 为上面又压栈了一个nil，所以负号去索引的话，要-1
	else
	{
		index = index - 1;
	}
	while (lua_next(L, index) != 0)
	{
		print_stack_value(L, -1);
		printf("\n");
		lua_pop(L, 1);
	}
}
