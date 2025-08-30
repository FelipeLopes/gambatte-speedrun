local M = require "posix.unistd"
local F = require "posix.fcntl"
local deque = require "deque"

local from_ws = F.open('/home/felipe/ws_to_lua_guest', bit32.bor(F.O_RDONLY, F.O_NONBLOCK))
local to_ws = F.open('/home/felipe/lua_to_ws_guest', F.O_WRONLY)

local host_started = false
local handshake_done = false
local q = deque.new()

function serial(a)
  sz = math.fmod(gb.read_memory(0xcba3) - gb.read_memory(0xcba4) + 32, 32)
  if not handshake_done then
    handshake_done = true
    return 0x29
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
  if q:is_empty() or sz == 31 then
    return 0xac
  else
    c = q:pop_left()
  end
  if c == 0x00 or c == 0xff then
    M.close(to_ws)
  end
  return c
end

function serial_on_timer()
  if not host_started then
    local data = M.read(from_ws, 1)
    if data ~= nil then
      host_started = true
      gb.request_serial_interrupt()
    end
  end
  local mode = gb.read_memory(0xcb74)
  if mode == 0x12 then
    gb.request_serial_interrupt()
  end
end

gb.on_serial_exchange(serial)
gb.on_timer_interrupt(serial_on_timer)
