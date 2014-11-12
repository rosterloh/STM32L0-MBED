openocd -f board/stm32l0discovery.cfg -c "init" -c "reset halt" -c "flash write_image erase build/DISCOVERY.bin 0x08000000" -c "reset run" -c "shutdown"
