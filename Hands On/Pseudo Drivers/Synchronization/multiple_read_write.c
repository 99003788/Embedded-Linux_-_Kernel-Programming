#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 64
int main() 
{ 
	int fd = 0 , nbytes =0;
	fd=open("/dev/psample0", O_RDWR);
	if(fd<0) 
	{
		perror("open");
	}
	char wbuf[]="sfdjvs";
	nbytes=write(fd,wbuf,7);
	if(nbytes == 0) 
	{
		perror("write");
	}
	char buf[32];
	int maxlen=10;
	while(1)
	{
		nbytes=read(fd,buf,maxlen);
		printf("%s",buf);
		if(nbytes == 0) 
		{
			break;
		}
		printf("%s",buf);
		write(1,buf,maxlen);
		buf[nbytes]='\0';
		
	}
	close(fd);
	return 0;
}
