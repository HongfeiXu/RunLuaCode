# Lua 源码学习

目录说明：

src中为Lua 5.4.3 的所有源码
thirdparty中为其他代码：
- [dirent.h](https://github.com/tronkko/dirent)

# Lua C API 学习

## Programming in Lua 4th ed.

> 代码在TestLua Project下面

utils

- LuaUtils.h
- LuaUtils.cpp

Chapter 27. An Overview of the C API

- SetLimit.h
- SimpleLuaInterpreter.h
- StackDump.h

Chapter 28. Extending Your Application

- CallALuaFunctionFromC.h
- GenericCallFunction.h
- LoadUserInfoFromFile.h

Chapter 29. Calling C from Lua

- CFunctionDir.h
- CFunctionsExample.h
- MyLib Project test.c
- EX_29.h

# Lua C\C++ binding

目标：

- [ ] 支持class
- [ ] 函数
- [ ] upvalue

