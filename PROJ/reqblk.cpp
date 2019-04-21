
#include <iostream>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <signal.h>
#define MSZ 5
using namespace std;
typedef struct Data {
  long to;
  long from;
  int data[MSZ];

} MESSAGE;
 
int mid;
key_t key;
struct msqid_ds buf;
MESSAGE msg;



void signal_callback_handler(int signum)
{
   printf("told to wake up");
}

int main(int argc, char** argv) {
   int blocksHave =0;
   int blockNumber,ch,dat;
   signal(SIGUSR1, signal_callback_handler);
   const int MYPID = getpid();
   int iD = getpid();
   int gpid  = atoi(argv[1]);
   long const GPID = (long)gpid;
    key = getuid();
    mid = msgget(key, 0);


  do{ 
  cout<<"get a block---\n ";
  cout<<"Enter Block Number : ";
  cin>>blockNumber;
  l0:
   cout<<"sending msg ...";
    msg.data[0] =-1;
    msg.data[1] =blockNumber;
    msg.data[2] = 0;
    msg.to = GPID;
    msg.from = MYPID;

    cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
    if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
        perror("msgsnd");
        exit(-1);
    }
 
    //Client waits for response from Server, throws an error if invalid input
    if(msgrcv(mid, &msg, sizeof(msg), iD, 0)<0){
        perror("msgrcv");
        exit(-1);
    }
    cout<<"recieved :"<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
    if(msg.data[2]==1)
    {
        cout<<msg.data[0];
    }
    if(msg.data[2]==-1)
     { cout<<"\nsleeping...";
      sleep(100);
      goto l0; }
   l5 :
   cout<<"-----Menu----\n";
   cout<<"1. set this block free :\n";
   cout<<"2. update this block set free :\n";
   cout<<"3. write this block to disk and set free :\n";
   cout<<"4. display buffer....\n";
   cout<<"5. exit ...\n";
   cout<<"6. kill getblock ....\n";
   
   cin>>ch;
   
    switch(ch)
    {

        case 1:        cout<<"\nsending msg ...";
                       msg.data[0] =-1;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 1;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;
        


        case 2:        cout<<"Enter data :";
                       cin>>dat; 
                       cout<<"\nsending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 2;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]"<<endl;
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;
               
        case 3:        cout<<"Enter data :";
                       cin>>dat; 
                       cout<<"\nsending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 3;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]"<<endl;
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;    
        case 4:       
                       cout<<"\nsending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 4;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]"<<endl;
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                       goto l5 ;
                    break;
        case 5:     
                       cout<<"\nsending msg ...";
                       msg.data[0] =-1;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 1;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                       ch =5;
                    break;      
        case 6:    cout<<"sending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 6;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]"<<endl;
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;


    }

  
   }while (ch<5);


  return EXIT_SUCCESS;
}



