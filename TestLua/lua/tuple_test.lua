x = tuple.new(10, "hi", {}, 3)
print(x(1)) --> 10
print(x(2)) --> hi
print(x()) --> 10 hi table: 0x8087878 3
print(x(300)) --> lua error: lua/tuple_test.lua:5: bad argument #1 to 'x' (index out of range)