local M = require "posix.unistd"
local F = require "posix.fcntl"
local deque = require "deque"

local from_ws = F.open('/home/felipe/ws_to_lua_host', bit32.bor(F.O_RDONLY, F.O_NONBLOCK))
local to_ws = F.open('/home/felipe/lua_to_ws_host', F.O_WRONLY)

local doing_handshake = true
local q = deque.new()

function serial(a)
  if doing_handshake then
    doing_handshake = false
    M.write(to_ws, string.char(0x29))
    return 0x12
  end
  if a ~= 0xac then
    M.write(to_ws, string.char(a))
  end
  local data = M.read(from_ws, 1)
  if data == nil then
    b = 0xac
  else
    b = string.byte(data)
    q:push_right(b)
  end
  if q:is_empty() then
    return 0xac
  else
    c = q:pop_left()
  end
  if c == 0x00 or c == 0xff then
    M.close(to_ws)
  end
  return c
end

gb.on_serial_exchange(serial)
