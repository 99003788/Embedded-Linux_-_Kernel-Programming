#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE0(mytestcall)
{	struct task_struct* task_list;
	for_each_process(task_list) 
{
	printk("\n %s %d \n",task_list->comm,task_list->pid);
	}
	return 0;
}
