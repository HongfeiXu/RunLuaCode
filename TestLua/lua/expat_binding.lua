local count = 0
callbacks = {
    StartElement = function (parser, tagname)
        io.write("+ ", string.rep("  ", count), tagname, "\n")
        count = count + 1
    end,

    EndElement = function(parser, tagname)
        io.write("- ", string.rep("  ", count), tagname, "\n")
        count = count + 1
    end
}

local lxp = require "lxp"
p = lxp.new(callbacks)
for i in io.lines() do
    assert(p:parse(1))
    assert(p:parse("\n"))
end

assert(p:parse())
p:close()
