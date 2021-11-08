local function overall()
    a = "one string one"
    b = string.gsub(a, "one", "fuck")
    print(b)

    print(#"hello")
    print(#"good bye")

    print("hello" .. "world")
    print("result is " .. 5)
end

-- overall()

--[[
fuck string fuck
5
8
helloworld
result is 5
--]]


local function literal_strings()
    print("one line\nnext line\n\"in quotes\", 'in quotes'")
    print("\x41")
    print("\u{3b1} \u{3b2} \u{3b3}")    -- The above example assumes an UTF-8 terminal
end

-- literal_strings()

--[[
one line
next line
"in quotes", 'in quotes'
A
伪 尾 纬
--]]


local function long_strings()
    s = [[
<html>
    <head>
        <title>An HTML Page</title>
    </head>
    <body>
        <a href="http://www.lua.org">Lua</a>
    </body>
</html>
]]
        
    print(s)
    
    print("this line\z
    \0continues")
end

-- long_strings()

--[[
<html>
    <head>
        <title>An HTML Page</title>
    </head>
    <body>
        <a href="http://www.lua.org">Lua</a>
    </body>
</html>

this linecontinues
--]]


local function coercions()
    -- converts number to string
    print(10 .. 20)
    print(type(10 .. 20))

    -- converts string to number
    print("10" + 1)

    print(tonumber("   -3   "))
    print(tonumber(" 10e4 "))
    print(tonumber("10e"))      -- nil
    print(tonumber("0x1.3p-4"))

    print(tonumber("100101", 2))
    print(tonumber("fff", 16))
    print(tonumber("-ZZ", 36))
    print(tonumber("987", 8))   -- nil

    print(tostring(10) == "10")

    -- print(2 < "15")  -- raise error: attempt to compare number with string
end

-- coercions()

--[[
1020
string
11
-3
100000.0
nil
0.07421875
37
4095
-1295
nil
true
--]]

-- [TODO]The String Library

local function string_library_test()
    print(string.rep("abc", 3))
    print(string.reverse("A Long Line!"))
    print(string.lower("A Long Line!"))
    print(string.upper("A Long Line!"))

    local s = "[in bracket]"
    print(string.sub(s, 2, -2))
    print(string.sub(s, 1, 1))
    print(string.sub(s, -1, -1))

    print(string.char(97))
    print(string.char(98, 99))
    print(string.byte("abcdef", 2))
    print(string.byte("helloworld", 1, -1)) -- a nice idiom

    print(string.format("x = %d y = %d", 10, 20))
    print(string.format("x = %x", 200))
    print(string.format("x = 0x%X", 200))
    local tag, title = "h1", "a title"
    print(string.format("<%s>%s<%s>", tag, title, tag))

    print(string.format("pi = %.4f", math.pi))
    local d, m, y = 5, 11, 1990
    print(string.format("%02d/%02d/%04d", d, m, y))

    print(s:sub(2, -2))

    print(string.find("hello world", "wor"))
    print(string.find("hello world", "war"))

    print(string.gsub("hello world", "l", "."))
end

-- string_library_test()

local function utf_8_test()
    print(utf8.len("你好"))     -- 2
    print(string.len("你好"))   -- 6
    print(utf8.char(114, 233))
    print(utf8.codepoint("你好", 1))

    for i, c in utf8.codes("你好") do
        print(i, c)
    end
end

utf_8_test()


