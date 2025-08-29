local deque = require "deque"

local q = deque.new()

q:push_left(1)
q:push_left(2)
q:push_left(3)

print(q:pop_right())
print(q:pop_right())
print(q:pop_right())

function print_rng()
  --print(gb.read_memory(0xcaca))
end

gb.on_timer_interrupt(print_rng)