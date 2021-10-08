a = array.new(5)

a[1] = true
a[3] = true
a[5] = true

b = array.new(4)
b[1] = true
b[4] = true

c = a + b

for i = 1, #c do
    print(c[i])
end
print("----")
d = a * b
for i = 1, #d do
    print(d[i])
end

print(tostring(a))

--[[
true
false
true
true
true
----
false
false
false
false

E:\BooksLearn\RunLuaCode\x64\Debug\TestLua.exe (进程 57172)已退出，代码为 0。
按任意键关闭此窗口. . .
--]]
