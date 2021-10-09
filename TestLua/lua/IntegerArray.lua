a = integerarray.new(3)
a[1] = 3
a[3] = 4
print(tostring(a))
-- a[4] = 6    -- lua error: lua/IntegerArray.lua:4: bad argument #2 to 'newindex' (index out of range)


--[[
=== integerarray start ===
size: 3
[1] = 3
[2] = 0
[3] = 4
=== integerarray end ===
--]]