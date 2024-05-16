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

	printf("1) device file open\n");

	dev = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
	if (dev >= 0) {
		printf("2) seek function call dev:%d\n", dev);
		ret = lseek(dev, 0x20, SEEK_SET);
		printf("ret = %08X\n", ret);

		printf("3) read functionn call\n");
		ret = read(dev, (char*)0x30, 0x31);
		printf("ret = %08X\n", ret);

		printf("4) write functionn call\n");
		//ret = write(dev, (char*)0x40, 0x41);
		do {
			buff[0] = 0x55;
			ret = write(dev, buff, 1);
			printf("ret = %08X\n", ret);
			sleep(1);

			buff[0] = 0x00;
			ret = write(dev, buff, 1);
			printf("ret = %08X\n", ret);
			sleep(1);
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