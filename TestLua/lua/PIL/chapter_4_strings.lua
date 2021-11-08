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

print(10 + "1")


-- [TODO]The String Library


