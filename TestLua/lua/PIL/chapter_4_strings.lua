a = "one string one"
b = string.gsub(a, "one", "fuck")
print(b)

print(#"hello")
print(#"good bye")

print("hello" .. "world")
print("result is " .. 5)

print("one line\nnext line\n\"in quotes\", 'in quotes'")

print("\x41")

print("\u{3b1} \u{3b2} \u{3b3}")    -- The above example assumes an UTF-8 terminal

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

print(10 .. 20) -- 数字与连接符之间需要空格，否则第一个点会被认为是小数点
print(type(10 .. 20))
