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

/*

99.0
c=      nil
b=      adas
a = 100.000000


1100.0
c=      nil
b=      adas
a = 1101.000000

E:\BooksLearn\LUA\ToLuaLearn\Debug\ToLuaSimpleTest.exe (���� 82340)���˳�������Ϊ 0��
��������رմ˴���. . .

*/

