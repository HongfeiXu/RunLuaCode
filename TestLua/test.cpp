#define USE_BINDING_CODE 0
#define USE_TEST_CODE 0


#if USE_BINDING_CODE

#include "LuaCallCFunc.h"
#include "CCallLuaFunc.h"

int main()
{
	Test_CCode_For_Lua();			// Lua ���� C ����
	Test_C_Call_LuaCode();			// C ���� Lua ����
	Test_C_Modify_Lua_Variable();	// C �޸� Lua ����

	return 0;
}

#elif USE_TEST_CODE

#include "TestLuaAPI.h"

int main()
{
	test_lua_api();

	return 0;
}

#else

#include "StackDump.h"

int main()
{
	stack_dump_test();
	return 0;
}

#endif


