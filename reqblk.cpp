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


void  disp(int &reqForm)
{  
   reqForm=5;

}


int main() {
   int blocksHave =0;
   int blockNumber,ch,dat;
   int S=PROCNUM;
   int fd = -1;
   int req ;
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

  do{ /*
    l:
                      while (atomic_load(&data->state) != 4) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) !=S)
                         {
                           goto l;
                             }

                       cout<<"Memory acquired\n";
                      disp(data->reqForm); 
                     cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
                    atomic_store(&data->state,5);*/

  cout<<"get a block---\n ";
  cout<<"Enter Block Number : ";
  cin>>blockNumber;
  l0:
   cout<<"trying to read ..";
    while (atomic_load(&data->state) != 4 ) { };// try to lock mem
    atomic_store(&data->state,S);
   if( atomic_load(&data->state) !=S)
      {
        goto l0;
      }

    printf("Block acquired\n");
    readandLock(data->blk_Num,data->reqForm,blockNumber); // messege Written 
   

    cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
   atomic_store(&data->state,5); //Messege Sent mem released
   
  while (atomic_load(&data->state) != 6) { } //Wait for reply
    dat = data->data;

   cout<<"Messege recv : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
   req=data->reqForm;
   if(req==-2)
    {cout<<"block not available"; }
    else
   cout<<"Data in block is :"<<dat;
    atomic_store(&data->state, 4); //release mem to all other requesting process

    if(req==-2)
     {sleep(30);
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

        case 1:       l1:
                      while (atomic_load(&data->state) != 4) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) != S)
                         {
                           goto l1;
                             }

                    cout<<"Memory acquired\n";
                    setFree(data->blk_Num,data->reqForm,blockNumber);// messege Written 
                    cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
                    atomic_store(&data->state,5); //Messege Sent mem released
                    break;
        
        case 2:     cout<< "Enter data: ";
                    cin>>dat;
                    l2:
                      while (atomic_load(&data->state) != 4 ) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) !=S)
                         {
                           goto l2;
                             }

                    cout<<"Memory acquired\n";
                    updateAndSetFree(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written
                   cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
                    atomic_store(&data->state,5); //Messege Sent mem released
                    break;

        case 3:     cout<< "Enter data: ";
                    cin>>dat;
                    l3:
                      while (atomic_load(&data->state) != 4 ) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) !=S)
                         {
                           goto l3;
                             }

                       cout<<"Memory acquired\n";
                    writeToDisk(data->data,data->blk_Num,data->reqForm,dat,blockNumber);// messege Written 
                    cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
                    atomic_store(&data->state,5); //Messege Sent mem released
                    break;
               
        case 4:     l4:
                      while (atomic_load(&data->state) != 4) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) !=S)
                         {
                           goto l4;
                             }

                       cout<<"Memory acquired\n";
                      disp(data->reqForm); 
                     cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
                    atomic_store(&data->state,5); //Messege Sent mem released
                    goto l5;
                     break;     
        case 5:    ch =5;
                    
                    //atomic_store(&data->state,-1); //Messege Sent mem released
                   break;
        case 6:    l6:
                      while (atomic_load(&data->state) != 4 ) {};// try to lock mem
                      atomic_store(&data->state,S);
                      if( atomic_load(&data->state) !=S)
                         {
                           goto l6;
                             }

                       cout<<"Memory acquired\n";
                         data->reqForm=-1;
                    atomic_store(&data->state, 5);
                   cout<<"Messege sent : ["<<data->data<<","<<data->blk_Num<<","<<data->reqForm<<"]";
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

 

