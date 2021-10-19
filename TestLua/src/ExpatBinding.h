#pragma once

// Refs: Programming in Lua, 4th ed. Ch32 Managing Resources


#include <stdio.h>
#include <expat\expat.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// -------------------------------------------------
// 整体流程框架
//
// lua 中定义回调函数表
// 回调函数表传到c，c这边对表中定义的回调进行lua_call
//
// c这边用userdata去表示XML_Parser，并存储用来解析的一个Lua state
// 提供创建、销毁、解析三个接口
// -------------------------------------------------


// use userdata to represent a parser in Lua
typedef struct lxp_userdata {
	XML_Parser parser;		// associated expat parser
	lua_State* L;			// store a lua state
}lxp_userdata;


// forward declarations for callback functions
static void f_StartElement(void* userData, const char* name, const char** atts);
static void f_EndElement(void* userData, const char* name);
static void f_CharData(void* userData, const char* s, int len);

// [C API]function to create XML parser objects
static int lxp_make_parser(lua_State* L)
{
	XML_Parser p;
	
	// 1. create a parser object (userdata)
	lxp_userdata* xpu = (lxp_userdata*)lua_newuserdata(L, sizeof(lxp_userdata));
	xpu->parser = NULL;	// pre-initialize it, in case of error

	// set its metatable
	luaL_getmetatable(L, "LuaBook.Expat");
	lua_setmetatable(L, -2);

	// 2. create the expat parser
	p = xpu->parser = XML_ParserCreate(NULL);
	if (!p)
	{
		luaL_error(L, "XML_ParserCreate failed");
	}

	// 3. check and store the callback table
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushvalue(L, 1);		// push table
	lua_setuservalue(L, -2);	// set it as the user value for the userdata

	

	// 4. config Expat parser
	XML_SetUserData(p, xpu);	// 舒服就舒服在expat支持uservalue，这样可以方便的把lxp_userdata传过去
	XML_SetElementHandler(p, f_StartElement, f_EndElement);
	XML_SetCharacterDataHandler(p, f_CharData);
	return 1;
}

// [C API]function to parse an XML fragment
static int lxp_parse(lua_State* L)
{
	// get and check first argument (should be a parser)
	lxp_userdata* xpu = (lxp_userdata*)luaL_checkudata(L, 1, "LuaBook.Expat");

	// check if it is not closed
	luaL_argcheck(L, xpu->parser != NULL, 1, "parser is closed");

	// get second argument (a string)
	size_t len;
	const char* s = luaL_optlstring(L, 2, NULL, &len);

	// put callback table at stack index 3
	lua_settop(L, 2);
	lua_getuservalue(L, 1);

	xpu->L = L;	// set Lua state:   [parser(userdata) - string - callback table]
	
	// call Expat to parse string
	int status = XML_Parse(xpu->parser, s, (int)len, s == NULL);	// will call the handlers for each relevant element
																	// that it finds in the given piece of document

	// return error code
	lua_pushboolean(L, status);
	return 1;
}

// [not C API]handler for character data
static void f_CharData(void* userData, const char* s, int len)
{
	lxp_userdata* xpu = (lxp_userdata*)userData;	// set by XML_SetUserData in lxp_make_parser
	lua_State* L = xpu->L;

	// get handler from callback table;
	lua_getfield(L, 3, "CharacterData");			// set by lxp_parse
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		return;
	}

	// call the Lua handler
	lua_pushvalue(L, 1);		// push the parser('self')
	lua_pushlstring(L, s, len);	// push char data
	lua_call(L, 2, 0);			// call the handler
}

// [not C API]handler for end elements
static void f_EndElement(void* userData, const char* name)
{
	lxp_userdata* xpu = (lxp_userdata*)userData;	// set by XML_SetUserData in lxp_make_parser
	lua_State* L = xpu->L;
	// get handler from callback table;
	lua_getfield(L, 3, "EndElement");			// set by lxp_parse
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		return;
	}
	// call the Lua handler
	lua_pushvalue(L, 1);		// push the parser('self')
	lua_pushstring(L, name);	// push tag name
	lua_call(L, 2, 0);			// call the handler
}

// [not C API]handler for start elements
static void f_StartElement(void* userData, const char* name, const char** atts)
{
	lxp_userdata* xpu = (lxp_userdata*)userData;	// set by XML_SetUserData in lxp_make_parser
	lua_State* L = xpu->L;
	// get handler from callback table;
	lua_getfield(L, 3, "StartElement");			// set by lxp_parse
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		return;
	}
	lua_pushvalue(L, 1);
	lua_pushstring(L, name);

	lua_newtable(L);
	for (; *atts; atts += 2)
	{
		lua_pushstring(L, *(atts + 1));
		lua_setfield(L, -2, *atts);		// table[*atts] = *(atts+1)
	}
	lua_call(L, 3, 0);
}

// [C API]close an xml parser
static int lxp_close(lua_State* L)
{
	// get and check first argument (should be a parser)
	lxp_userdata* xpu = (lxp_userdata*)luaL_checkudata(L, 1, "LuaBook.Expat");

	if (xpu->parser)
	{
		XML_ParserFree(xpu->parser);
	}
	xpu->parser = NULL;
	return 0;
}

// // Make a C Module

static const struct luaL_Reg lxplib[] = {
	{"new", lxp_make_parser},
	{NULL, NULL}
};

static const struct luaL_Reg lxplib_method[] = {
	{"parse", lxp_parse},
	{"close", lxp_close},
	{"__gc", lxp_close},
	{NULL, NULL}
};

int luaopen_lxplib(lua_State* L)
{
	// create a metatable
	luaL_newmetatable(L, "LuaBook.Expat");

	// metatable.__index = metatable
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, lxplib_method, 0);	// 和上面两行等价效果

	luaL_newlib(L, lxplib);
	return 1;
}

void require_lxplib(lua_State* L)
{
	luaL_requiref(L, "lxp", luaopen_lxplib, 1);
	lua_pop(L, 1);
}

// test
void test()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	require_lxplib(L);
	if (luaL_dofile(L, "lua/expat_binding.lua") != LUA_OK)
	{
		printf("lua error: %s\n", lua_tostring(L, -1));
	}
	lua_close(L);
}
