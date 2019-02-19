#include<fcntl.h>
void main()
{
int fd;
char libuf[20],bigbuf[1024];
fd=open("file",O_RDONLY);
read(fd,libuf,20);
read(fd,bigbuf,1024);
read(fd,libuf,20);
printf("lil buf printed :\n");
for(int charloc=0;charloc<20;charloc++)
{
		printf(libuf);
}
printf("big buf printed :\n");
for(int charloc=0;charloc<1024;charloc++)
{
		printf(libuf);
}
return;
}cat file
