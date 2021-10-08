a = array.new(1000)

a:set(2, true)
print(a:size())
print(a:get(2))
print(a:get(3))
print(a:__tostring())

--[[
1000
true
false
array(1000)

G:\Projects\RunLuaCode\x64\Debug\TestLua.exe (process 65940) exited with code 0.
Press any key to close this window . . .
--]]
