
local function number_type()
    print(type(3))
    print(type(3.0))
    print(3 == 3.0)
    print(0.2e3 == 200)

    print(math.type(3))
    print(math.type(3.0))
end

-- number_type()

-- number
-- number
-- true
-- true
-- integer
-- float

local function division_test()
    print(4/2)
    print(4.0/2)

    print(5//2)
    print(5.0//2)
    print(-9//2)
end

-- division_test()

-- 2.0
-- 2.0
-- 2
-- 2.0
-- -5

local function module_test()
    print(-10 % 3)

    local x = math.pi
    print (x - x % 0.01)    -- 取浮点数的小数点后几位
    print (x - x % 0.001)
end

-- module_test()

-- 2
-- 3.14
-- 3.141

local function relational_operators_test()
    print(1 ~= 2)   -- 不等号
end

-- relational_operators_test()

-- true

local function math_lib_test()
    print(math.sin(math.pi/2))
    print(math.max(3,2,100,-19))
    print(math.huge)

    -- math.random的三种调用方式
    print(math.random())
    print(math.random(7))
    print(math.random(1, 10))

    -- 相同seed得到的值一定相同
    math.randomseed(1)
    print(math.random(10))
    math.randomseed(1)
    print(math.random(10))

    -- floor\ceil\modf
    print(math.floor(3.3))
    print(math.floor(-3.3))
    print(math.ceil(3.3))
    print(math.ceil(-3.3))
    print(math.modf(3.3))
    print(math.modf(-3.3))
    print(math.floor(2^70))

end

-- round a number x to the nearest integer
local function round(x)
    local f = math.floor(x)
    if x== f then   -- treat integral values separately
        return f
    else 
        return math.floor(x + 0.5)
    end
end


-- unbiased rounding: rounds half-integers to the nearest event integer
local function unbiased_round(x)
    local f = math.floor(x)
    if (x == f) or (x % 2.0 == 0.5) then
        return f
    else
        return math.floor(x + 0.5)
    end
end

local function round_test()
    print(round(2.5))
    print(round(3.5))

    print(unbiased_round(2.5))
    print(unbiased_round(3.5))
end

-- round_test()

-- 3
-- 4
-- 2
-- 4

local function representation_limits()
    print(math.maxinteger + 1 == math.mininteger)
    print(math.mininteger - 1 == math.maxinteger)
    print(-math.mininteger == math.mininteger)
    print(math.maxinteger)
    print(math.mininteger)
    print(12.7 - 20 + 7.3)  -- not 0
    print(math.maxinteger + 2.0 == math.maxinteger + 1.0)   -- true
end

-- representation_limits()

-- true
-- true
-- true
-- 9223372036854775807
-- -9223372036854775808
-- -8.8817841970013e-16
-- true

local function conversions()
    -- force a number to be a float(a + 0.0)
    print(-3 + 0.0)
    print(9007199254740991 + 0.0 == 9007199254740991)
    print(9007199254740992 + 0.0 == 9007199254740992)
    print(9007199254740993 + 0.0 == 9007199254740993)   -- false

    -- force a number to be an integer(a | 0) ；仅适用于a这个浮点数有对应的整数（相等，且在整数范围内）
    print(2^53 | 0)
    print((2^53 | 0) == 2^53)

    -- print(3.2 | 0)
    -- C:\lua-5.4.3\src\lua.exe: ...cts\LUA\RunLuaCode\TestLua\lua\PIL\chapter_3_numbers.lua:152: number has no integer representation
    -- stack traceback:...

    -- force a number to an integer by math.tointeger
    print(math.tointeger(-258.0))
    print(math.tointeger(2^30))
    print(math.tointeger(5.01)) -- nil
    print(math.tointeger(2^64)) -- nil
end

conversions()

