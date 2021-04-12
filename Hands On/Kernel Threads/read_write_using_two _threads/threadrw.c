#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define SIZE 64

void* task_writer(void *pv)
{
  int nbytes;
  int fd = (int)pv;
  char wbuf[SIZE];
  memset(wbuf, 0, SIZE*sizeof(char));
  nbytes=write(fd,wbuf,SIZE);
  if(nbytes<0)
  {
    perror("write");
  }
}

void* task_reader(void *pv)
{
  int nbytes;
  int fd = (int)pv;
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
}

int main()
{
  int fd;
  pthread_t p1;
  pthread_t p2;
  fd=open("/dev/psample0",O_RDWR);
  if(fd<0)
  {
     perror("open");
  }
  pthread_create(&p1,NULL,task_writer,(void*)fd);
  pthread_create(&p2,NULL,task_reader,(void*)fd);
  close(fd);
  return 0;
}
