#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILENAME "/dev/calldev"

int main(void) {
	int dev;
	char buff[128];
	int ret;
	unsigned int key_data, key_data_old=0;
	int i;

	printf("1) device file open\n");

	dev = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
	if (dev >= 0) {
		printf("2) seek function call dev:%d\n", dev);
		ret = lseek(dev, 0x20, SEEK_SET);
		printf("ret = %08X\n", ret);

		printf("3) read functionn call\n");
		//ret = read(dev, (char*)0x30, 0x31);
		//printf("ret = %08X\n", ret);

		printf("4) write functionn call\n");
		//ret = write(dev, (char*)0x40, 0x41);
		do {
			usleep(100000);
			read(dev, buff, 1);	
			key_data = buff[0];
			if(key_data != key_data_old)
			{
				key_data_old = key_data;
				if(key_data)
				{
					write(dev, buff, 1);

					puts("0:1:2:3:4:5:6:7");
					for(i=0;i<8;i++)
					{
						if(key_data & (0x01 << i))
							putchar('O');
						else
							putchar('X');
						if(i != 7 )
							putchar(':');
						else
							putchar('\n');
					}
					putchar('\n');
				}
				if(key_data == 0x80)
					break;
			}
		} while (1);

		printf("5) ioctl functionn call\n");
		ret = ioctl(dev, 0x51, 0x52);
		printf("ret = %08X\n", ret);

		printf("6) device file close\n");
		ret = close(dev);
		printf("ret = %08X\n", ret);
	}
	else 
		perror("open");
	
	return 0;
}
