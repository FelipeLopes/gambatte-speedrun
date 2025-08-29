function serial(a)
  if a == 0x29 then
    return 0x12
  else
    return a
  end
end

gb.on_serial_exchange(serial)
