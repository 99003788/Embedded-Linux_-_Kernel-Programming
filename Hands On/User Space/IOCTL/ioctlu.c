#include<stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include<linux/ioctl.h>
#define IOC_MAGIC 'p'
#define MY_IOCTL_RESET  _IO(IOC_MAGIC, 3)
#define MY_IOCTL_AVAIL  _IO(IOC_MAGIC, 2)
#define MY_IOCTL_LEN	_IO(IOC_MAGIC, 1)

int main()
{
	int nbytes,ret;
	printf("hel;lohdjfh");
	int fd=open("/dev/psample0",O_RDWR);
	printf("\n%d\n",fd);
	if(fd<0)
		perror("open");
	char str[]="abcdxyz";
	ret=ioctl(fd, MY_IOCTL_LEN);
	if(ret<0)
	{
		perror("ioctl");
		exit(3);
	}
	ret=ioctl(fd, MY_IOCTL_AVAIL);
	if(ret<0)
	{
		perror("ioctl");
		exit(3);
	}
	ret=ioctl(fd, MY_IOCTL_RESET);
	if(ret<0) 
	{
		perror("ioctl");
		exit(3);
	}


	if(nbytes<0)
	{
		perror("read");
	}
	close(fd);
}
