#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int ret;
	ret = mknod("/dev/test", S_IRWXU | S_IRWXG | S_IRWXO | S_IFCHR, (240 << 8) | 1);
	// S_IRWX- : 권한 부여
	// S_IFCHR : 문자 디바이스

	if (ret < 0) {
		perror("mknod()");
		return EACCES; // 일반적으로는 음수 리턴
	}

	ret = open("/dev/test", O_RDONLY);
	if (ret < 0) {
		perror("open()");
		return 2;
	}

	return 0;
}
