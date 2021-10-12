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

// use userdata to represent a parser in Lua
typedef struct lxp_userdata {
	XML_Parser parser;		// associated expat parser
	lua_State* L;			// store a lua state
}lxp_userdata;


// forward declarations for callback functions

//typedef void(XMLCALL* XML_StartElementHandler)(void* userData,
//	const XML_Char* name,
//	const XML_Char** atts);

static void f_StartElement(void* userData, const char* name, const char** atts);
static void f_EndElement(void* userData, const char* name);
static void f_CharData(void* userData, const char* s, int len);

// function to create XML parser objects
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
	lua_pushvalue(L, 1);	// push table
	lua_setuservalue(L, -2);	// set it as the user value for the userdata

	// 4. config Expat parser
	XML_SetUserData(p, xpu);
	XML_SetElementHandler(p, f_StartElement, f_EndElement);
	XML_SetCharacterDataHandler(p, f_CharData);
	return 1;
}

// TODO: function to parse an XML fragment



