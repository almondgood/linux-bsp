#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

#define LEDKEY_DEV_NAME	"ledkeydev"
#define LEDKEY_DEV_MAJOR	230
#define DEBUG 1

static int gpioLedInit(void);
static void gpioLedSet(long val);
static void gpioLedFree(void);

static int gpioKeyInit(void);
static int gpioKeyGet(void); //return 0x00 ~ 0xff
static void gpioKeyFree(void);
static int gpioKeyIrqInit(void);
static void gpioKeyIrqFree(void);

static int gpioLed[] = {6, 7, 8, 9, 10, 11, 12, 13};
static int gpioKey[] = {16,17,18,19,20,21,22,23};
static int key_irq[8];
static int keyNumber=0;

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

static int gpioKeyIrqInit(void) {
	int i;

	for (i = 0; i < 8; i++) {
		key_irq[i] = gpio_to_irq(gpioKey[i]);
		if (key_irq[i] < 0) {
			printk("Failed gpio_to_irq() gpio%d error\n", gpioKey[i]);
			return key_irq[i];
		}
	}
	return 0;
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

static void gpioKeyIrqFree(void) {
	int i;
	for (i = 0; i < 8; i++) {
		free_irq(key_irq[i], NULL);
	}
}

static int ledkey_open(struct inode *inode, struct file *filp) {
	int num = MINOR(inode->i_rdev);
	printk("ledkey open-> minor : %d\n", num);
	num = MAJOR(inode->i_rdev);
	printk("ledkey open-> major : %d\n", num);
	return 0;
}
static ssize_t ledkey_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
	//char kbuf[128];
	//int i;
	char kbuf;
	//printk("ledkey read -> buf : %08X, count : %08X \n", (unsigned int)buf, count);
	//kbuf[0] = gpioKeyGet();
	/*
	for (i=0; i<count; i++) 
		put_user(kbuf[i], buf++);
	*/	
	kbuf = keyNumber;
	put_user(kbuf, buf);
	keyNumber = 0;
	//i = copy_to_user(buf, kbuf, count);

	return count;
}
static ssize_t ledkey_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
	char kbuf[128];
	int i;
	//printk("ledkey write -> buf : %08X, count : %08X \n", (unsigned int)buf, count);
	//gpioLedSet(*buf);
	/*
	for (i=0;i<count;i++)
		get_user(kbuf[i], buf++);
	*/
	i = copy_from_user(kbuf, buf, count);
	gpioLedSet(*kbuf);

	return count;
}
static long ledkey_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) { 
	//printk("ledkey ioctl -> cmd : %08X, arg : %08X\n", cmd, (unsigned int)arg);
	return 0x53;
}
static int ledkey_release(struct inode *inode, struct file *filp) {
	printk("ledkey release \n");
	return 0;
}

irqreturn_t key_isr(int irq, void* data){
	int i;
	for (i=0;i<8;i++){
		if(irq == key_irq[i]) {
			keyNumber = i+1;
			break;
		}
	}
#if DEBUG
	printk("key_isr() irq : %d, keyNumber : %d\n", irq,   keyNumber);
#endif
	return IRQ_HANDLED;
}

struct file_operations ledkey_fops = {
	.owner	= THIS_MODULE,
	.read	= ledkey_read,
	.write	= ledkey_write,
	.unlocked_ioctl	= ledkey_ioctl,
	.open	= ledkey_open,
	.release	= ledkey_release,
};
static int ledkey_init(void) {
	int i;
	int result;
	char* irqName[8] = {"irqKey0","irqKey1","irqKey2","irqKey3","irqKey4","irqKey5","irqKey6","irqKey7"};

	printk("ledkey ledkey_init \n");

	result = register_chrdev(LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME, &ledkey_fops);
	if (result < 0) return result;

	result = gpioLedInit();
    if (result < 0) 
		return result;

	result = gpioKeyInit();
    if (result < 0) 
		return result;

	result = gpioKeyIrqInit();
    if (result < 0) 
		return result;

	for (i=0;i<8;i++){
		result = request_irq(key_irq[i], key_isr, IRQF_TRIGGER_RISING, irqName[i], NULL); // NULL : free_irq의 NULL과 같음
		if (result < 0) {
			printk("request_irq() failed irq %d\n", key_irq[i]);
			return result;
		}
	}

	return 0;
	
}
static void ledkey_exit(void) {
	printk("ledkey ledkey_exit \n");
	unregister_chrdev(LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME);
	gpioLedSet(0x00);
	gpioKeyIrqFree();
	gpioKeyFree();
	gpioLedFree();
}

module_init(ledkey_init);
module_exit(ledkey_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("ledkey module");
MODULE_AUTHOR("KCCI-AIOT");
