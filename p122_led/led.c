#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/gpio.h>

static int onevalue = 1;
static char* twostring = NULL;
module_param(onevalue, int, 0);
module_param(twostring, charp, 0);

static int gpioLed[] = {6, 7, 8, 9, 10, 11, 12, 13};
static int gpioKey[] = {16,17,18,19,20,21,22,23};

static int led_init(void); 
static void led_exit(void);

static int gpioLedInit(void);
static void gpioLedSet(long val);
static void gpioLedFree(void);

static int gpioKeyInit(void);
static int gpioKeyGet(void); //return 0x00 ~ 0xff
static void gpioKeyFree(void);

asmlinkage long sys_mysyscall(long val) {
//	printk(KERN_INFO "Welcome to KCCI's Embedded System!! app value=%ld\n", val);
//	return val*val;

	int ret = 0;
	ret = gpioLedInit();
	if (ret < 0) {
		return ret;
	}

	gpioLedSet(val);
	gpioLedFree();

/*
	ret = gpioKeyInit();
	if (ret < 0) {
		return ret;
	}

	val = gpioKeyGet();
	gpioKeyFree();
*/
	return val;
}

static int gpioLedInit() {
	int i;
	int ret = 0;
	char gpioName[10];

	for (i = 0; i < 8; i++) {
		sprintf(gpioName, "led%d", i);
		ret = gpio_request(gpioLed[i], gpioName);
		if (ret < 0) {
			printk("Failed request gpio%d error\n", gpioLed[i]);
			return ret;
		}
	}

	for (i = 0; i < 8; i++) {
		ret = gpio_direction_output(gpioLed[i], 0);  // led off
		if (ret < 0) {
			printk("Failed direction output gpio%d error\n", gpioLed[i]);
			return ret;
		}
	}

	return ret;
}

static void gpioLedSet(long val) {
	int i;
	for (i = 0; i < 8; i++) {
		gpio_set_value(gpioLed[i], val & (0x01 << i));  // led on
	}
}

static void gpioLedFree() {
	int i;
	for (i = 0; i < 8; i++) {
		gpio_free(gpioLed[i]);
	}
}

static int gpioKeyInit(void) {
	int i;
	int ret = 0;
	char gpioName[10];

	for (i = 0; i < 8; i++) {
		sprintf(gpioName, "key%d", i);
		ret = gpio_request(gpioKey[i], gpioName);
		if (ret < 0) {
			printk("Failed request gpio%d error\n", gpioKey[i]);
			return ret;
		}
	}

	for (i = 0; i < 8; i++) {
		ret = gpio_direction_input(gpioKey[i]);  // key off
		if (ret < 0) {
			printk("Failed direction input gpio%d error\n", gpioKey[i]);
			return ret;
		}
	}

	return ret;
}

static int gpioKeyGet(void) {
	int i;
	int val = 0x00, key = 0;
	for (i = 7; i >= 0; i--) {
		key = gpio_get_value(gpioKey[i]);  // led on
		val = val | (key << i);
	}
	return val;
}

static void gpioKeyFree(void) {
	int i;
	for (i = 0; i < 8; i++) {
		gpio_free(gpioKey[i]);
	}
}

static int led_init(void) {
	int ret;
	printk("Hello, World! [onevalue=%d, twostring=%s]\n", onevalue, twostring);
	ret = gpioLedInit();
	if (ret < 0) {
		//perror("gpioLedInit()");
		return ret;
	}
	gpioLedSet(onevalue);

	return 0;
}

static void led_exit(void) {
	printk("Goodbye, World\n");
	gpioLedSet(0x00);
	gpioLedFree();
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("Dual BSD/GPL");
