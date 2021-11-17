print(MLuaCar.price)
print(MLuaCar)

for k, v in pairs(MLuaCar) do
	print(k, v)
end

MLuaCar.price = 200
print(MLuaCar.price)