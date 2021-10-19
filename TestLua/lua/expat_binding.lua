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
f_StartElement finish
--stack_dump begin--
userdata   '<themeSortOrder method="post" priority="high">123</themeSortOrder>' 
--stack_dump end--
data:   123
f_CharData finish
--stack_dump begin--
userdata   '<themeSortOrder method="post" priority="high">123</themeSortOrder>' 
--stack_dump end--
endTag:         themeSortOrder
f_EndElement finish
--stack_dump begin--
userdata   '<themeSortOrder method="post" priority="high">123</themeSortOrder>' 
--stack_dump end--
--]]