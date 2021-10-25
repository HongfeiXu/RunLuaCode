-- defines a factorial function

function fact(n)
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

print(type(nil))
print(type(true))
print(type(10.4 * 3))
print(type("Hello world"))
print(type(io.stdin))
print(type(print))
print(type(type))
print(type({}))
print(type(type(X)))


