/*
** Lua binding: Bindlua
*/

#include "tolua.h"

#ifndef __cplusplus
#include <stdlib.h>
#endif
#ifdef __cplusplus
 extern "C" int tolua_bnd_takeownership (lua_State* L); // from tolua_map.c
#else
 int tolua_bnd_takeownership (lua_State* L); /* from tolua_map.c */
#endif
#include <string.h>

/* Exported function */
TOLUA_API int tolua_Bindlua_open (lua_State* tolua_S);
LUALIB_API int luaopen_Bindlua (lua_State* tolua_S);

double a = 20;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* get function: a */
static int tolua_get_a(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)a);
 return 1;
}

/* set function: a */
static int tolua_set_a(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  a = ((double)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* Open lib function */
LUALIB_API int luaopen_Bindlua (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
 tolua_variable(tolua_S,"a",tolua_get_a,tolua_set_a);
 tolua_endmodule(tolua_S);
 return 1;
}
/* Open tolua function */
TOLUA_API int tolua_Bindlua_open (lua_State* tolua_S)
{
 lua_pushcfunction(tolua_S, luaopen_Bindlua);
 lua_pushstring(tolua_S, "Bindlua");
 lua_call(tolua_S, 1, 0);
 return 1;
}
