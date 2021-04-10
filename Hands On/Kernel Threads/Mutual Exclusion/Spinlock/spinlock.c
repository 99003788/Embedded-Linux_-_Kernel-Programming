#include <linux/init.h>
#include<linux/delay.h>
#include<linux/syscalls.h>
#include<linux/module.h>
#include<linux/kthread.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>

static struct task_struct *task1;
static struct task_struct *task2;
int val=100;
const int max=10;
static DEFINE_SPINLOCK(s1);
static int thread_one(void *pargs)
{
	int i;
	printk("---> Thread A Welcome<---");
	spin_lock(&s1);
	for(i=1;i<=max;i++)
	{
		val++;
		printk("Thread A");
	}
	spin_unlock(&s1);
	do_exit(0);
	return 0;
}

static int thread_two(void *pargs)
{
	int i;
	printk("---> Thread B Welcome<---");
	spin_lock(&s1);
	for(i=1;i<=max;i++)
	{
		val--;
		printk("Thread B");
	}
	spin_unlock(&s1);
	do_exit(0);
	return 0;
}

static int __init spinlock_init(void) 
{  
	spin_lock_init(&s1);
	task1=kthread_run(thread_one, NULL, "thread_A");
	task2=kthread_run(thread_two, NULL, "thread_B");
	return 0;
}

static void __exit spinlock_exit(void)
{
	printk("VAL:%d",val);
	printk("----EXIT----");
}

module_init(spinlock_init);
module_exit(spinlock_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suvradeep");
MODULE_DESCRIPTION("Mutual Exclusion");
