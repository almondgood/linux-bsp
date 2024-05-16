cmd_/home/ubuntu/pi_bsp/drivers/p122_led/led.mod := printf '%s\n'   led.o | awk '!x[$$0]++ { print("/home/ubuntu/pi_bsp/drivers/p122_led/"$$0) }' > /home/ubuntu/pi_bsp/drivers/p122_led/led.mod
