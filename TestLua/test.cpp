#include "LuaCallCFunc.h"
#include "CCallLuaFunc.h"

int main()
{
	//Test_CCode_For_Lua();		// Lua ���� C ����

	//Test_C_Call_LuaCode();		// C ���� Lua ����
	Test_C_Modify_Lua_Variable();	// C �޸� Lua ����
	return 0;
}