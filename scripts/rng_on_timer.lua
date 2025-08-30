local M = require "posix.unistd"
local F = require "posix.fcntl"

local from_ws = F.open('/home/felipe/ws_to_lua', bit32.bor(F.O_RDONLY, F.O_NONBLOCK))

function print_rng()
  local data, err = M.read(from_ws, 1)
  if data ~= nil then
    print(data)
  end
end

gb.on_timer_interrupt(print_rng)