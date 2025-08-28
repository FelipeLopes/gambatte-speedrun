function print_rng()
  print(gb.read_memory(0xcaca))
end

gb.on_timer_interrupt(print_rng)