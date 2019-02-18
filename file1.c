#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

int fdrd,fdwt;
char c;
main(argc,argv)
int argc;
char* argv[];
{
if (argc!=3)
	exit(1);
//printf("arguments okay");
if((fdrd=open(argv[1],O_RDONLY))==-1)
	exit(1);
//printf("file opened");	
if((fdwt=creat(argv[2],0666))==-1)
	exit(1);

fork();

rdwrt();
exit(0);
}

rdwrt()
{ //    printf("in rdwrt");
	for(;;)
		{
		if(read(fdrd,&c,1)!=1)
		return;
		write(fdwt,&c,1);
	//	printf("reached here2");

		}	
}
