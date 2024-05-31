#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>          
#include <linux/errno.h>       
#include <linux/types.h>       
#include <linux/fcntl.h>       
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>

#define DEBUG 0
#define LEDKEY_DEV_NAME            "ledkey_dev"
#define LEDKEY_DEV_MAJOR            230      

#define OFF 0
#define ON 1
#define GPIOLEDCNT 8
#define GPIOKEYCNT 8
static int gpioLed[GPIOLEDCNT] = {6,7,8,9,10,11,12,13};
static int gpioKey[GPIOKEYCNT] = {16,17,18,19,20,21,22,23};
typedef struct {
	int key_irq[8];
	int keyNumber;
} keyData;

static int gpioLedInit(void);
static void gpioLedSet(long);
static void gpioLedFree(void);
static int gpioKeyInit(void);
//static int gpioKeyGet(void);
static void gpioKeyFree(void);
static int gpioKeyIrqInit(keyData * pKeyData);
static void gpioKeyIrqFree(keyData * pKeyData);

DECLARE_WAIT_QUEUE_HEAD(WaitQueue_Read);

static irqreturn_t key_isr(int irq, void *data)
{
	int i;
	keyData * pKeyData = (keyData *)data;
	for(i=0;i<GPIOKEYCNT;i++)
	{
		if(irq == pKeyData->key_irq[i])
		{
			pKeyData->keyNumber = i+1;
			break;
		}
	}
#if DEBUG
	printk("key_isr() irq : %d, KeyNumber : %d\n",irq, pKeyData->keyNumber);
#endif
	wake_up_interruptible(&WaitQueue_Read);
	return IRQ_HANDLED;	
}
static int gpioLedInit(void)
{
	int i;
	int ret=0;
	char gpioName[10];
	for(i=0;i<GPIOLEDCNT;i++)
	{
		sprintf(gpioName,"led%d",i);
		ret = gpio_request(gpioLed[i],gpioName);
		if(ret < 0) {
			printk("Failed gpio_request() gpio%d error \n",i);
			return ret;
		}

		ret = gpio_direction_output(gpioLed[i],OFF);
		if(ret < 0) {
			printk("Failed gpio_direction_output() gpio%d error \n",i);
			return ret;
		}
	}
	return ret;
}

static void gpioLedSet(long val)
{
	int i;
	for(i=0;i<GPIOLEDCNT;i++)
	{
		gpio_set_value(gpioLed[i],(val>>i) & 0x1);
	}
}
static void gpioLedFree(void)
{
	int i;
	for(i=0;i<GPIOLEDCNT;i++)
	{
		gpio_free(gpioLed[i]);
	}
}
static int gpioKeyInit(void)
{
	int i;
	int ret=0;
	char gpioName[10];
	for(i=0;i<GPIOKEYCNT;i++)
	{
		sprintf(gpioName,"key%d",gpioKey[i]);
		ret = gpio_request(gpioKey[i], gpioName);
		if(ret < 0) {
			printk("Failed Request gpio%d error\n", 6);
			return ret;
		}
	}
	for(i=0;i<GPIOKEYCNT;i++)
	{
		ret = gpio_direction_input(gpioKey[i]);
		if(ret < 0) {
			printk("Failed direction_output gpio%d error\n", 6);
       	 	return ret;
		}
	}
	return ret;
}
/*
static int	gpioKeyGet(void)
{
	int i;
	int ret;
	int keyData=0;
	for(i=0;i<GPIOKEYCNT;i++)
	{
//		ret=gpio_get_value(gpioKey[i]) << i;
//		keyData |= ret;
		ret=gpio_get_value(gpioKey[i]);
		keyData = keyData | ( ret << i );
	}
	return keyData;
}
*/
static void gpioKeyFree(void)
{
	int i;
	for(i=0;i<GPIOKEYCNT;i++)
	{
		gpio_free(gpioKey[i]);
	}
}

static int gpioKeyIrqInit(keyData * pKeyData)
{

	int i;
	int result;
	char * irqName[8] = {"IrqKey0","IrqKey1","IrqKey2","IrqKey3","IrqKey4","IrqKey5","IrqKey6","IrqKey7"};
	for(i=0;i<GPIOKEYCNT;i++)
	{
		pKeyData->key_irq[i] = gpio_to_irq(gpioKey[i]);
		if(pKeyData->key_irq[i] < 0)
		{
			printk("gpioKeyIrq() Failed gpio %d\n",gpioKey[i]);
			return pKeyData->key_irq[i];
		}
	}
	for(i=0;i<GPIOKEYCNT;i++)
	{
		result = request_irq(pKeyData->key_irq[i],key_isr,IRQF_TRIGGER_RISING,irqName[i],pKeyData);
		if(result < 0)
		{
			printk("request_irq() failed irq %d\n",pKeyData->key_irq[i]);
			return result;
		}
	}
	return 0;
}

static void gpioKeyIrqFree(keyData * pKeyData)
{
	int i;
	for(i=0;i<GPIOKEYCNT;i++)
	{
		free_irq(pKeyData->key_irq[i],pKeyData);
	}
}

static int ledkey_open (struct inode *inode, struct file *filp)
{
	int result;
    int num0 = MAJOR(inode->i_rdev); 
    int num1 = MINOR(inode->i_rdev); 
	keyData * pKeyData = (keyData *)kmalloc(sizeof(keyData),GFP_KERNEL);	
	if(!pKeyData)
		return -ENOMEM;
	pKeyData->keyNumber = 0;

#if DEBUG
    printk( "ledkey open -> major : %d\n", num0 );
    printk( "ledkey open -> minor : %d\n", num1 );
#endif
	try_module_get(THIS_MODULE);


	result=gpioLedInit();
	if(result < 0)
		return result;
	result=gpioKeyInit();
	if(result < 0)
		return result;
	result = gpioKeyIrqInit(pKeyData);
	if(result < 0)
		return result;

	filp->private_data = pKeyData;

    return 0;
}

static ssize_t ledkey_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
//	int result;
//    char kbuf;
	keyData * pKeyData = (keyData *)filp->private_data;
#if DEBUG
    printk( "ledkey read -> buf : %08X, count : %08X \n", (unsigned int)buf, count );
#endif 
//    kbuf = (char)gpioKeyGet();
//	kbuf = pKeyData->keyNumber;
	
	if(pKeyData->keyNumber == 0)
	{
		if(!(filp->f_flags & O_NONBLOCK))	
		{
			wait_event_interruptible(WaitQueue_Read,pKeyData->keyNumber);
//			wait_event_interruptible_timeout(WaitQueue_Read,pKeyData->keyNumber,100);     //100 * 1/HZ = 100 * 1/100 = 100 * 0.01 = 1Sec

		}
	}

    put_user(pKeyData->keyNumber,buf);
//  result = copy_to_user(buf, &(pKeyData->keyNumber), count);
	if(pKeyData->keyNumber)
		pKeyData->keyNumber = 0;
    return count;
}

static ssize_t ledkey_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
//	int i;
//	int result;
	char kbuff;
/*	char kbuff[10];
	for(i=0;i<count;i++)
		get_user(kbuff[i],buf++);
*/
/*	char kbuff[10];
	copy_from_user(kbuff,buf,count);
*/
#if DEBUG
    printk( "ledkey write -> buf : %08X, count : %08X \n", (unsigned int)buf, count );
#endif
  	get_user(kbuff,buf);
//	result = copy_from_user(&kbuff,buf,count);
	gpioLedSet(kbuff);	
    return count;
}
static __poll_t ledkey_poll(struct file * filp, struct poll_table_struct * wait)
{

	unsigned int mask=0;
    keyData * pKeyData = (keyData *)filp->private_data;
#ifdef DEBUG
    printk("_key : %u\n",(poll_requested_events(wait) & POLLIN));
#endif
    if(poll_requested_events(wait) & POLLIN)
        poll_wait(filp, &WaitQueue_Read, wait);
    if(pKeyData->keyNumber > 0)
        mask = POLLIN;

    return mask;

}
static long ledkey_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{

#if DEBUG
    printk( "ledkey ioctl -> cmd : %08X, arg : %08X \n", cmd, (unsigned int)arg );
#endif
    return 0x53;
}

static int ledkey_release (struct inode *inode, struct file *filp)
{
	keyData * pKeyData = (keyData *)filp->private_data;
#if DEBUG
    printk( "ledkey release \n" );
#endif
	module_put(THIS_MODULE);
	gpioKeyIrqFree(pKeyData);
    gpioKeyFree();
	gpioLedFree();
	if(pKeyData)
		kfree(pKeyData);
    return 0;
}

static struct file_operations ledkey_fops =
{
//    .owner    = THIS_MODULE,
    .open     = ledkey_open,     
    .read     = ledkey_read,     
    .write    = ledkey_write,    
	.poll	  = ledkey_poll,
	.unlocked_ioctl = ledkey_ioctl,
    .release  = ledkey_release,  
};

static int ledkey_init(void)
{
    int result;

    printk( "ledkey ledkey_init \n" );    

    result = register_chrdev( LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME, &ledkey_fops);
    if (result < 0) return result;

    return 0;
}

static void ledkey_exit(void)
{
    printk( "ledkey ledkey_exit \n" );    
    unregister_chrdev( LEDKEY_DEV_MAJOR, LEDKEY_DEV_NAME );
}

module_init(ledkey_init);
module_exit(ledkey_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("KCCI-AIOT KSH");
MODULE_DESCRIPTION("led key test module");
