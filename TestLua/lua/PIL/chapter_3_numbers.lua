
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

module_test()

-- 2
-- 3.14
-- 3.141