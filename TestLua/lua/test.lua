LuaFileStr = "hello lua string"
LuaFileTable = {name = "Hongfei", age = 27}
function LuaFileAdd(n1, n2)
	return (n1 + n2);
end
function ShowTable()
	print("Name:"..LuaFileTable.name);
	print("Age:"..LuaFileTable.age);
end
