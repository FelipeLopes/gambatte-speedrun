local M = require "posix.unistd"
local F = require "posix.fcntl"

local from_ws = F.open('/home/felipe/ws_to_lua', bit32.bor(F.O_RDONLY, F.O_NONBLOCK))
local to_ws = F.open('/home/felipe/lua_to_ws', F.O_WRONLY)

local a = 0

function print_rng()
  a = math.fmod(a+1,240)
  local data, err = M.read(from_ws, 1)
  if data ~= nil then
    print(data)
  end
  if a == 0 then
    M.write(to_ws, string.char(gb.read_memory(0xcaca)))
  end
end

gb.on_timer_interrupt(print_rng)