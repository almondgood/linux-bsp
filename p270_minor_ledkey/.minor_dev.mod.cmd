cmd_/home/ubuntu/pi_bsp/drivers/p270_minor_ledkey/minor_dev.mod := printf '%s\n'   minor_dev.o | awk '!x[$$0]++ { print("/home/ubuntu/pi_bsp/drivers/p270_minor_ledkey/"$$0) }' > /home/ubuntu/pi_bsp/drivers/p270_minor_ledkey/minor_dev.mod