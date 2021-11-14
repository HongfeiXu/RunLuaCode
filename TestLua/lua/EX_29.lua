-- ex 29.1

print(exlib.summation()) --> 0
print(exlib.summation(2.3, 5.4)) --> 7.7
print(exlib.summation(2.3, 5.4, -34)) --> -26.3
-- print(exlib.summation(2.3, 5.4, {})) --要看报错解开注释

-- ex 29.2
t = exlib.pack("a","b","c")
for k, v in pairs(t) do
	print(k, v)
end

-- ex 29.3
print(exlib.reverse("a", 1, "kill", "enjoy"))

-- ex 29.4
exlib.foreach({ x = 10, y = 20 }, print)


--[[

0.0
7.7
-26.3
1       1
2       2
3       3
n       3
enjoy   kill    1       a
x       10
y       20

--]]

