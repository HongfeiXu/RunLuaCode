lproc.start([[
print("c1 send begin")
lproc.send("c1")
print("c1 sended ,send c2")
lproc.send("c2")
print("send c2 end")
]])
lproc.start([[
print("c1 receive begin")
lproc.receive("c1")
print("c1 received ,receive c2")
lproc.receive("c2")
print("receive c2 end")
]])

--[[
c1 send begin
c1 receive begin
c1 received ,receive c2
c1 sended ,send c2
send c2 end
receive c2 end
--]]