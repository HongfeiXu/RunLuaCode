#include <stdio.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void testBindLua();
void testMLuaCar();


int main()
{
	testMLuaCar();
}

