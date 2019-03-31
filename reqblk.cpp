#include "protocolp.h"
#include <iostream>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;
#define PROCNUM 0

void readandLock(int &blk_Num,int &reqForm,int bn)
{
     blk_Num = bn;
     reqForm =  0;

}

void setFree(int &blk_Num,int &reqForm,int bn)
{
  blk_Num = bn;
  reqForm = 1;
}

void  updateAndSetFree(int &data,int &blk_Num,int &reqForm,int d ,int bn)
{
   data = d ;
   blk_Num = bn;
   reqForm =2;
}

void  writeToDisk(int &data,int &blk_Num,int &reqForm,int d ,int bn)
{  
   data = d ;
   blk_Num = bn;
   reqForm =3;

}



int main() {
   
   int blockNumber,ch,dat;
   int fd = -1;
  while (fd == -1) {
    fd = shm_open(NAME, O_RDWR, 0666);
    if (fd < 0 && errno != ENOENT) {
      perror("shm_open()");
      return EXIT_FAILURE;
    }
  }
  
  ftruncate(fd, SIZE);

  struct Data *data = (struct Data *)
      mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("worker: mapped address: %p\n", data);

  printf("worker: waiting initial data\n");
//

  do{

  cout<<"get a block : ";
  cout<<"Enter Block Number : ";
  cin>>blockNumber;
    while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
    atomic_store(&data->state, PROCNUM); //acquire block
    printf("Block acquired\n");
    readandLock(data->blk_Num,data->reqForm,blockNumber); // messege Written
   
   atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
  while (atomic_load(&data->state) != PROCNUM) {} //Wait for reply
    atomic_store(&data->state, PROCNUM); //acquire block
    dat = data->data;
   cout<<"Data in block is :"<<dat;
    atomic_store(&data->state, PROCNUM+4); //release block

   cout<<"-----Menu----";
   cout<<"1. set this block free :";
   cout<<"2. update this block set free :";
   cout<<"3. write this block to disk and set free :";
   cout<<"4. display buffer....";
   cout<<"5. exit ...";
   cout<<"6. kill getblock ....";
   
   cin>>ch;
   
    switch(ch)
    {

        case 1:     while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    setFree(data->blk_Num,data->reqForm,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;
        
        case 2:     cout<< "Enter data: ";
                    cin>>dat;
                    while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    updateAndSetFree(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;

        case 3:     cout<< "Enter data: ";
                    cin>>dat;
                    while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
                    atomic_store(&data->state, PROCNUM); //acquire block
                    printf("Block acquired\n");
                    writeToDisk(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written 
                    atomic_store(&data->state, PROCNUM+8); //Messege Sent block released
                    break;
               
        case 4:     while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
                    atomic_store(&data->state, PROCNUM);
                    atomic_store(&data->state, 101);
                   break;     
        case 5:    ch =5;
                   break;
        case 6:    while (atomic_load(&data->state) != PROCNUM) {};//wait for turn
                    atomic_store(&data->state, PROCNUM);
                    atomic_store(&data->state, 100);
                    ch=4;
                   break;


    }

  
   }while (ch<5);


 // printf("worker: release updated data\n");

  //atomic_store(&data->state, 6);
  
  munmap(data, SIZE);

  close(fd);

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

 

