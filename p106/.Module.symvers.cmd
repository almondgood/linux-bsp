cmd_/home/ubuntu/pi_bsp/drivers/p106/Module.symvers :=  sed 's/ko$$/o/'  /home/ubuntu/pi_bsp/drivers/p106/modules.order | scripts/mod/modpost -m -a    -o /home/ubuntu/pi_bsp/drivers/p106/Module.symvers -e -i Module.symvers -T - 
