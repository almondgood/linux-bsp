cmd_/home/ubuntu/pi_bsp/drivers/p184_ledkey/call_dev.mod := printf '%s\n'   call_dev.o | awk '!x[$$0]++ { print("/home/ubuntu/pi_bsp/drivers/p184_ledkey/"$$0) }' > /home/ubuntu/pi_bsp/drivers/p184_ledkey/call_dev.mod