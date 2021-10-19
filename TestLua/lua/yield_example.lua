function foo(x)
    coroutine.yield(10, x)
end

function fool(x)
    foo(x + 1)
    return 3
end

