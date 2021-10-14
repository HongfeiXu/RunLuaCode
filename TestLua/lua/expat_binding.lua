callbacks = {
    StartElement = function (parser, tagname, attsTable)
        print("startTag: ", tagname)
        for k, v in pairs(attsTable) do
            print("attr: ", k, v)
        end
    end,

    EndElement = function(parser, tagname)
        print("endTag: ", tagname)
    end,

    CharacterData = function(parser, charData)
        print("data: ", charData)
    end

}

local lxp = require "lxp"
p = lxp.new(callbacks)
for l in io.lines() do
    assert(p:parse(l))
    assert(p:parse("\n"))
end

assert(p:parse())
p:close()

--[[
<themeSortOrder method="post" priority="high">123</themeSortOrder>
startTag:       themeSortOrder
attr:   priority        high
attr:   method  post
data:   123
endTag:         themeSortOrder
--]]