#include "LuaCallCFunc.h"
#include "CCallLuaFunc.h"

int main()
{
	//Test_CCode_For_Lua();		// Lua 调用 C 函数

	//Test_C_Call_LuaCode();		// C 调用 Lua 函数
	Test_C_Modify_Lua_Variable();	// C 修改 Lua 变量
	return 0;
}