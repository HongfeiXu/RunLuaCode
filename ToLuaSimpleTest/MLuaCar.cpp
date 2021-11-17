#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
extern "C" int tolua_bnd_takeownership(lua_State * L); // from tolua_map.c
#else
int tolua_bnd_takeownership(lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

double price = 100;

static int tolua_get_price(lua_State* tolua_S)
{
	tolua_pushnumber(tolua_S, (lua_Number)price);
	return 1;
}

static int tolua_set_price(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err))
		tolua_error(tolua_S, "#vinvalid type in variable assignment.", &tolua_err);
#endif
	price = ((double)tolua_tonumber(tolua_S, 2, 0));
	return 0;
}

int tolua_module_car(lua_State* tolua_S)
{
	tolua_module(tolua_S, "MLuaCar", 1);
	tolua_beginmodule(tolua_S, "MLuaCar");

	tolua_variable(tolua_S, "price", tolua_get_price, tolua_set_price);

	tolua_endmodule(tolua_S);
	return 1;
}

int luaopen_Car(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	tolua_module(tolua_S, NULL, 1);
	tolua_beginmodule(tolua_S, NULL);

	tolua_module_car(tolua_S);

	tolua_endmodule(tolua_S);

	return 1;
}