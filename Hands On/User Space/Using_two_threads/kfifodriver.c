#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kfifo.h>
#include<linux/wait.h>
#include <linux/kthread.h>

static struct task_struct *task1;
static struct task_struct *task2;

#define MAX_SIZE 1024

struct cdev cdev;
dev_t pdevid;
int ndevices=1;

wait_queue_head_t rd_queue;
struct device *pdev; //global
struct class *pclass; //global
struct kfifo kfifo;

unsigned char *pbuffer;

int pseudo_open(struct inode* inode , struct file* file)
{
  printk("Pseudo--open method\n");
  return 0;
}
int pseudo_close(struct inode* inode , struct file* file)
{
  printk("Pseudo--release method\n");
  return 0;
}
ssize_t pseudo_write(struct file * file, const char __user * ubuf , size_t size, loff_t * off)
{
  int wcount,ret;
  char *tbuf;
  if(kfifo_is_full(&kfifo))
  {
     printk("buffer is full\n");
     return -ENOSPC;
   }
   wcount = size;
   if(wcount > kfifo_avail(&kfifo))
   {
     wcount = kfifo_avail(&kfifo);
   }
   tbuf=kmalloc(wcount, GFP_KERNEL);
   ret=copy_from_user(tbuf, ubuf, wcount);
   if(ret)
   {
      printk("copy from user failed\n");
      return -EFAULT;
   }
   kfifo_in(&kfifo, tbuf, wcount);
   kfree(tbuf);
   wake_up_interruptible(&rd_queue);
   printk("Pseudo--write method\n");
   return wcount;
}

ssize_t pseudo_read(struct file * file, char __user * ubuf , size_t size, loff_t * off)
{
  int rcount,ret;
  char *tbuf;
  if(kfifo_is_empty(&kfifo))
  {
    wait_event_interruptible(rd_queue,(kfifo_len(&kfifo)>0));
   }
   rcount = size;
   if(rcount > kfifo_len(&kfifo))
   {
     rcount = kfifo_len(&kfifo);
   }
   tbuf = kmalloc(rcount, GFP_KERNEL);
   kfifo_out(&kfifo, tbuf, rcount);
   ret=copy_to_user(ubuf, tbuf, rcount);
   if(ret)
   {
     printk("copy to user failed\n");
     return -EFAULT;
   }
   kfree(tbuf);
   printk("Pseudo--read method\n");
   return rcount;
}

struct file_operations fops = {
   .open    = pseudo_open,
   .read    = pseudo_read,
   .write   = pseudo_write,
   .release = pseudo_close
    };


static int __init psuedo_init(void)
{
   int ret;
   int i=0;
   pbuffer=kmalloc(MAX_SIZE, GFP_KERNEL);
   kfifo_init(&kfifo, pbuffer,MAX_SIZE);
   init_waitqueue_head(&rd_queue);
   pclass = class_create(THIS_MODULE, "pseudo_class");
   ret=alloc_chrdev_region(&pdevid, 0, ndevices, "myfiledriver_sample");
   if(ret) 
   {
      printk("Pseudo: Failed to register driver\n");
      return -EINVAL;
   }
   cdev_init(&cdev, &fops);
   kobject_set_name(&cdev.kobj,"pdevice%d", i);
   ret = cdev_add(&cdev, pdevid, 1);
   pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d",i);
   printk("Successfully registered,major=%d,minor=%d\n",MAJOR(pdevid), MINOR(pdevid));//MAKEDEV
   printk("Pseudo Driver Sample..welcome\n");
   return 0;
}

static void __exit psuedo_exit(void)
{
  kfifo_free(&kfifo);
  device_destroy(pclass, pdevid);
  cdev_del(&cdev);
  unregister_chrdev_region(pdevid, ndevices);
  class_destroy(pclass);
  printk("Pseudo Driver Sample..Bye\n");
}


module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suvradeep");
MODULE_DESCRIPTION("A Simple Driver");

