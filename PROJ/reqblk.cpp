
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
    cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
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

        case 1:        cout<<"sending msg ...";
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
                       cout<<"sending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 2;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;
               
        case 3:        cout<<"Enter data :";
                       cin>>dat; 
                       cout<<"sending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 3;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;    
        case 4:        cout<<"Enter data :";
                       cin>>dat; 
                       cout<<"sending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 4;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;
        case 5:     ch =5;
                    break;      
        case 6:    cout<<"sending msg ...";
                       msg.data[0] = dat;
                       msg.data[1] =blockNumber;
                       msg.data[2] = 4;
                       msg.to = GPID;
                       msg.from = MYPID;
                       cout<<"["<<msg.to<<","<<msg.from<<","<<msg.data[0]<<","<<msg.data[1]<<","<<msg.data[2]<<"]";
                       if(msgsnd(mid, &msg, sizeof(msg.data), 0)==-1){
                           perror("msgsnd");
                           exit(-1);
                       }
                    break;


    }

  
   }while (ch<5);


 // printf("worker: release updated data\n");

  //atomic_store(&data->state, 6);
  


  return EXIT_SUCCESS;
}


/*
void menu()
{ 
  

  do{

  cout<<"get a block : ";
  cout<<"Enter Block Number : "
  cin>>blockNumber;
    while (atomic_load(&data->state) != PROCNUM+4) {};//wait for turn
    atomic_store(&data->state, PROCNUM); //acquire block
    printf("Block acquired\n");
    readandLock(data->blk_Num,data->reqForm,blockNumber); // messege Written
   
   atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
  while (atomic_load(&data->state) != PROCNUM) {} //Wait for reply
    atomic_store(&data->state, 0); //acquire block
    dat = data->data;
   cout<<"Data in block is :"<<dat;
    atomic_store(&data->state, PROCNUM+5); //release block

   cout<<"-----Menu----";
   cout<<"1. set this block free :";
   cout<<"2. update this block set free :";
   cout<<"3. write this block to disk and set free :";
   cout<<"4. exit ...";
   cout<<"5. kill getblock ....";
   cin>>ch;
   
    switch(ch)
    {

        case 1:     while (atomic_load(&data->state) != PROCNUM+4) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    setFree(data->blk_Num,data->reqForm,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;
        
        case 2:     cout<< "Enter data: ";
                    cin>dat;
                    while (atomic_load(&data->state) != PROCNUM+4) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    updateAndSetFree(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;

        case 3:     cout<< "Enter data: ";
                    cin>dat;
                    while (atomic_load(&data->state) != PROCNUM+4) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    writeToDisk(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;
               
               
        case 4:    ch =4;
                   break;
        case 5:    while (atomic_load(&data->state) != PROCNUM+4) {};//wait for turn
                    atomic_store(&data->state, PROCNUM);
                    atomic_store(&data->state, 100);
                    ch=4;
                   break;


    }

  





   }while (ch<4);
}

*/

 

