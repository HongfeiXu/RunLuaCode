local M = {}

function M.dump_array(t, depth, indent)
    return M.dump(t, depth, indent, ipairs)
end

function M.dump_dict(t, depth, indent)
    return M.dump(t, depth, indent, pairs)
end

function M.dump(t, depth, indent, iterator)
    depth = depth or 0
    indent = indent or "  "
    local str = ""
    if type(t) ~= "table" then
        return tostring(t)
    end
    
    local cur_depth = depth
    local cur_prefix = ""
    while cur_depth > 0 do
        cur_prefix = cur_prefix..indent
        cur_depth = cur_depth - 1
    end
    str = str..("%s{\n"):format(cur_prefix)
    for k, v in iterator(t) do
        str = str..("%s%s : %s,\n"):format(cur_prefix..indent, tostring(k), M.dump(v, depth+1, indent, iterator))
    end
    str = str..("%s}\n"):format(cur_prefix)
    return str
end

function M.split(s, p)
    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt
 end

return M