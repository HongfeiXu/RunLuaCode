#define USE_BINDING_CODE 0


#if USE_BINDING_CODE

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

#include "EX_29.h"

int main(int argc, char* argv[])
{
	test();

	return 0;
}

#endif


