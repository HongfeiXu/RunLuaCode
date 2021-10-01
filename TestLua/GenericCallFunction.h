#pragma once

// Refs: Programming in Lua, 4th ed. Ch28 Extending Your Application

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "LuaUtils.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


/////////////////////////////////////////////////////
// A generic call function
// takes the name of a global function to be called, 
// a string describing the types of the argumentsand results, 
// then the list of arguments, and finally a list of
// pointers to variables to store the results.
// “Calling a Lua function from C” simply like this:
// call_va(L, "f", "dd>d", x, y, &z)

void call_va(lua_State* L, const char* func, const char* sig, ...)
{
    va_list vl;
    int narg, nres;             // number of arguments and results

    va_start(vl, sig);
    lua_getglobal(L, func);     // push function

    // push and count arguments
    for (narg = 0; *sig; narg++)    // repeat for each argument
    {
        // check stack space
        luaL_checkstack(L, 1, "too many arguments");

        switch (*sig++) 
        {
        case 'd':
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':
            lua_pushinteger(L, va_arg(vl, int));
            break;
        case 's':
            lua_pushstring(L, va_arg(vl, char*));
            break;
        case '>':
            goto endargs;
        default:
            simple_error_handler(L, "invalid opetion (%C)", *(sig - 1));
        }
    }
    endargs:

    nres = strlen(sig); // number of excepted results

    // do the call
    if (lua_pcall(L, narg, nres, 0) != LUA_OK)
    {
        simple_error_handler(L, "error calling '%s': %s", func, lua_tostring(L, -1));
    }

    // retrieve results
    nres = -nres;   // stack index of the first result（因为函数返回值是正序压栈的）
    while (*sig)    // repeat for each result
    {
        switch (*sig++)
        {
        case 'd':
        {
            int isnum;
            double n = lua_tonumberx(L, nres, &isnum);
            if (!isnum)
            {
                simple_error_handler(L, "wrong result type");
            }
            *va_arg(vl, double*) = n;
            break;
        }
        case 'i':
        {
            int isint;
            int n = lua_tointegerx(L, nres, &isint);
            if (!isint)
            {
                simple_error_handler(L, "wrong result type");
            }
            *va_arg(vl, int*) = n;
            break;
        }
        case 's':
        {
            const char* s = lua_tostring(L, nres);
            if (s == NULL)
            {
                simple_error_handler(L, "wrong result type");
            }
            *va_arg(vl, const char**) = s;
            break;
        }
        default:
            simple_error_handler(L, "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }
    va_end(vl);
}

void call_va_test()
{
    lua_State* L = luaL_newstate();

    luaL_openlibs(L);
    load_lua_src(L, "functions.lua");

    // f(3, 3)
    double f_ret;
    double x = 3, y = 3;
    call_va(L, "f", "dd>d", x, y, &f_ret);  
    printf("result = %lf\n", f_ret);

    // f2("hello ", "world")
    const char* str1 = "hello ";
    const char* str2 = "world";
    const char* result = NULL;
    call_va(L, "f2", "ss>s", str1, str2, &result);
    printf("result = %s\n", result);

    lua_close(L);
}

/*
result = -0.635040
result = hello world
*/


// stdarg.h 学习
void printargs(int arg1, ...) /* 輸出所有int型態的參數，直到-1結束 */
{
    va_list ap;
    int i;

    va_start(ap, arg1);
    for (i = arg1; i != -1; i = va_arg(ap, int))
        printf("%d ", i);
    va_end(ap);
    putchar('\n');
}

void do_printargs(void)
{
    printargs(5, 2, 1, 84, 97, 15, 24, 48, -1);
    printargs(84, 51, -1);
    printargs(-1);
    printargs(1, -1);
}