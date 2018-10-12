#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
  int fd, k;
  char str[5];
  umask(0);
  if((fd = open("hello.c", O_RDONLY, 0666)) < 0)
  {  
    printf("Can`t open file\n");
    exit(-1);
  }
  while( (k = read(fd, str, 5)) != 0)
  {
    write(1, str, k);
    //write(1, "\n", 1);
  }
    //printf("%s\n", str);
  if (close(fd) < 0)
  {
    printf("Can`t close file\n");
    exit(-1);
   }
  return 0;
} 
