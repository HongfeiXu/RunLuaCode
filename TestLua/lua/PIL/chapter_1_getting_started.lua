-- defines a factorial function

local function fact(n)
    if n == 0  then
        return 1
    else
        return n * fact(n - 1)
    end
end

--[[
print("enter a number:")
a = io.read("*n")
print(fact(a))
--]]

--[[
PS G:\Projects\RunLuaCode\TestLua\lua\PIL> lua -i .\chapter_1_getting_started.lua
Lua 5.4.3  Copyright (C) 1994-2021 Lua.org, PUC-Rio
> fact(2) 
2 
> 
--]]

local function basic_types()
    print(type(nil))
    print(type(true))
    print(type(10.4 * 3))
    print(type("Hello world"))
    print(type(io.stdin))
    print(type(print))
    print(type(type))
    print(type({}))
    print(type(type(X)))
end

-- basic_types()

-- nil
-- boolean
-- number
-- string
-- userdata
-- function
-- function
-- table
-- string


local function boolean_test()
    print(4 and 5)
    print(nil and 13)
    print(false and 13)
    print(0 or 5)
    print(false or "hi")
    print(nil or false)
end

-- boolean_test()

-- 5
-- nil
-- false
-- 0
-- hi
-- false

local function boolean_test2()
    print(3 and 4 or 5)
    print(nil and 4 or 5)

    print(not nil)
    print(not 1)
    print(not 2)
end

-- boolean_test2()

-- 4
-- 5
-- true
-- false
-- false

-- Exercise 1.8: Write a simple script that prints its own name without knowing it in advance.

local function print_script_name()
    if arg and arg[0] then
        print(arg[0])
    end

    for k, v in pairs(arg) do
        print(k, v)
    end
end

print_script_name()

--[[
PS G:\Projects\LUA\RunLuaCode\TestLua\lua\PIL> lua "g:\Projects\LUA\RunLuaCode\TestLua\lua\PIL\chapter_1_getting_started.lua"
g:\Projects\LUA\RunLuaCode\TestLua\lua\PIL\chapter_1_getting_started.lua
0       g:\Projects\LUA\RunLuaCode\TestLua\lua\PIL\chapter_1_getting_started.lua
-1      C:\lua-5.4.3\src\lua.exe
--]]

