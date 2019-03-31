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
#define PROCNUM 1

void readandLock(int &blk_Num,int &reqForm,int bn)
{
     blk_Num = bn;
     reqForm =  0;

}

void setFree(int &blk_Num,int &reqForm,int d ,int bn)
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
   int num;
   int req;
   int state;
   int blockNumber=21;
   int dataInBlock=11;
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
for(int i=0;i<5;i++)
{  sleep(1);
  while (atomic_load(&data->state) != 5) {};
    atomic_store(&data->state, PROCNUM);
    printf("worker: acquire initial data\n");
    
    printf("worker: update data\n");
    blockNumber=i;
    readandLock(data->blk_Num,data->reqForm,blockNumber);  //request sent
    req=data->reqForm;
    atomic_store(&data->state, PROCNUM+6);
    dataInBlock=41;
    

  while (atomic_load(&data->state) != PROCNUM) {} //data recieved
    //atomic_store(&data->state, 0);
   
    dataInBlock = data->data;


    //updateAndSetFree(data->data,data->blk_Num,data->reqForm,dataInBlock,blockNumber);
    if(req==0)
    atomic_store(&data->state, 5);
    else
    atomic_store(&data->state, PROCNUM+6);

 } 


  //printf("worker: release updated data\n");
  //atomic_store(&data->state, 6);
  
  munmap(data, SIZE);

  close(fd);

  return EXIT_SUCCESS;
}