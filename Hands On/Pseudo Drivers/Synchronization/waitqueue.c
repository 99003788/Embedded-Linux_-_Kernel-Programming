#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include<linux/wait.h>

#define MAX_SIZE 1024

struct device *pdev;
struct class *pclass;
unsigned char *pbuffer;
dev_t pdevid;
struct cdev cdev;
int ndevices = 1;
struct kfifo myfifo;
wait_queue_head_t wr_queue;


int pseudo_open(struct inode *inode, struct file *file) {
  printk("Pseudo--open method\n");
  return 0;
}

int pseudo_close(struct inode *inode, struct file *file) {
  printk("Pseudo--release method\n");
  return 0;
}

ssize_t pseudo_read(struct file *file, char __user *ubuf, size_t size,loff_t *off) 
{
   int ret,rcount;
  if(kfifo_is_empty(&myfifo))
   {
     printk("buffer is empty\n");
     return 0;
    } 
   //min
   rcount = size;
   if(rcount > kfifo_len(&myfifo))
    rcount = kfifo_len(&myfifo);
  //  min
  char *tbuf = kmalloc(rcount, GFP_KERNEL);
  kfifo_out(&myfifo, tbuf, rcount);
  ret=copy_to_user(ubuf, tbuf, rcount);
  //error handling
  kfree(tbuf);
  wake_up_interruptible(&wr_queue);
  return rcount;
}

ssize_t pseudo_write(struct file *file, const char __user *ubuf, size_t size,loff_t *off) 
{
   int ret,wcount;
  if(kfifo_is_empty(&myfifo))
   {
    wait_event_interruptible(wr_queue,(kfifo_len(&myfifo)>0));
   }  
   wcount = size;
   if(wcount > kfifo_avail(&myfifo))
    {
     wcount = kfifo_avail(&myfifo);
     }
   char *tbuf=kmalloc(wcount, GFP_KERNEL);
   ret=copy_from_user(tbuf, ubuf, wcount);
   //error handling if copy_form_user 
   kfifo_in(&myfifo, tbuf, wcount);
    kfree(tbuf);
    return wcount;
}


struct file_operations fops = {.open = pseudo_open,
                               .release = pseudo_close,
                               .write = pseudo_write,
                               .read = pseudo_read};



static int __init psuedo_init(void) {
  int ret;
  int i = 0;
  pclass = class_create(THIS_MODULE, "pseudo_class");
  
  ret = alloc_chrdev_region(&pdevid, 0, ndevices, "pseudo_sample");
  if (ret) {
    printk("Pseudo: Failed to register driver\n");
    return -EINVAL;
  }
  pbuffer = kmalloc(MAX_SIZE, GFP_KERNEL);
  kfifo_init(&myfifo, pbuffer,MAX_SIZE);
  
 // kfifo_alloc(&myfifo, MAX_SIZE, GFP_KERNEL);
  
  cdev_init(&cdev, &fops);
  kobject_set_name(&cdev.kobj, "pdevice%d", i);
  ret = cdev_add(&cdev, pdevid, 1);
  pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d",i);
  init_waitqueue_head(&wr_queue);
  printk("Successfully registered,major=%d,minor=%d\n", MAJOR(pdevid),
         MINOR(pdevid));
  printk("Pseudo Driver Sample..welcome\n");
  return 0;
}
static void __exit psuedo_exit(void) {
  
  device_destroy(pclass, pdevid);
  cdev_del(&cdev);
  unregister_chrdev_region(pdevid, ndevices);
  class_destroy(pclass);
  //kfifo_free(myfifo);
  printk("Pseudo Driver Sample..Bye\n");
}

module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suvradeep");
MODULE_DESCRIPTION("Register Char Driver");
