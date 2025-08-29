local doing_handshake = true

function serial(a)
  if doing_handshake then
    doing_handshake = false
    return 0x29
  else
    return a
  end
end

function serial_on_timer()
  gb.request_serial_interrupt()
end

gb.on_serial_exchange(serial)
gb.on_timer_interrupt(serial_on_timer)
