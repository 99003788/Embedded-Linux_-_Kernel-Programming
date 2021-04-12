#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 64
int main()
{
  int fd,nbytes;
  fd=open("/dev/psample0",O_RDWR);
  if(fd<0)
  {
     perror("open");
  }
  char wbuf[SIZE];
  memset(wbuf, 0, SIZE*sizeof(char));
  nbytes=write(fd,wbuf,SIZE);
  if(nbytes<0)
  {
    perror("write");
  }
  char rbuff[SIZE];
  while(1)
  {
    nbytes=read(fd,rbuff,10);
    if(nbytes < 0)
    {
        perror("read");
    } 
    if(nbytes == 0)
    {
       break;
    }
    rbuff[nbytes]='\0';
    printf("\n%s nbytes=%d", rbuff,nbytes);
  }
  close(fd);
  return 0;
}
