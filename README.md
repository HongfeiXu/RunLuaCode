# Lua 源码学习

目录说明：

src中为Lua 5.4.3 的所有源码
thirdparty中为其他代码：
- [dirent.h](https://github.com/tronkko/dirent)

thirdpartylibs中为第三方库的lib

- expat的x64是cmake编译出的，Win32是github上下载的

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

Chapter 30. Techniques for Writing C Functions

- CFunctionMap.h
- CFunctionSplit.h
- CFunctionStrUpper.h
- CFunctionTableConcat.h
- CFunctionUpvalues.h
- ImpTupleByUpValue.h

Chapter 31.  User-Defined Types in C

- BooleanArray.h
- IntegerArray.h

Chapter 32. Managing Resources

- DirIterator.h
- ExpatBinding.h

# Lua C\C++ binding


目标：

- [ ] 支持class
- [ ] 函数
- [ ] upvalue

