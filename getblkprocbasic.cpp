
#include "protocolp.h"
#include <iostream>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include<cstdio>
#include <ctime>    // For time()
#include<cstdlib>

#define HASHQNUM 4
#define BUFBLOCKNUM 20
#define DISKSIZE 50
using namespace std;
int d=0;




 struct BufferBlock {
                      int diskBlockNum;
                      int data;
                      bool isFree;
                      bool dataWritten;
                      BufferBlock* nextBlock;
                      BufferBlock* previousBlock;
                      BufferBlock* nextInFreeList;
                      BufferBlock* previousInFreeList;

                        } *freeList, *freeListEnd, *hashQueue[HASHQNUM];

class Buffer {
                      //BufferBlock = new *hashQueue[HASHQNUM];
                      //BufferBlock *hashQueue = BufferBlock[HASHQNUM];
                      //BufferBlock* freeList;
                    int *Disk;

                  public:

                     Buffer();
                     void createDisk();
                     void insertBlock(BufferBlock* &temp);
                     void createBufferBlock();
                     void display();
                     int countFreeBuffer();
                     void removeFromFreeList(BufferBlock* &freeBlock);
                     void removeFromHashQueue(BufferBlock* &freeBlock);
                     void displayQueue(int queueNum);
                     BufferBlock* bread(int blockRNum);
                     void  bwrite(BufferBlock* block);
                     bool searchInHq(int qNum,int diskBlkNum);
                     BufferBlock* searchBlkInHq(int diskBlkNum); 
                      BufferBlock* getBlock(int blockNum);

                     void actOnRequest( int &data,int &blk_Num,int &reqForm);

};

 Buffer::Buffer()
                      {   
                          cout<<"construction started...";
                          
                          freeList = NULL;
                          Disk= NULL;
                          createDisk();
                          
                          for(int i=0;i<BUFBLOCKNUM;i++)
                          {
                              createBufferBlock();
                         
                          } 

                         cout<<"Buffer Initialized";

                      }


void Buffer:: createDisk()
{
   Disk = new int[DISKSIZE];
   for(int i =0 ;i<DISKSIZE;i++)
   {
    Disk[i]=5;
   }

}

void Buffer::createBufferBlock()

{

      
    BufferBlock *tempBlock = new BufferBlock;
    tempBlock->data =d++;
    tempBlock->diskBlockNum       = -1;
    tempBlock->isFree             = true;
    tempBlock->dataWritten        = true;
    tempBlock->nextBlock          = NULL;
    tempBlock->previousBlock      = NULL;
    tempBlock->nextInFreeList     = freeList ;
    tempBlock->previousInFreeList = freeList ;
    insertBlock(tempBlock);
    return ;

}                      


void Buffer::insertBlock( BufferBlock* &temp)

{


    //struct BufferBlock *temp;

    //temp = createBufferBlock();

    if (freeList == freeListEnd && freeList == NULL)

    {    

        cout<<"||";

        freeList = freeListEnd = temp;

        freeList->nextInFreeList = freeListEnd->nextInFreeList =temp;

        freeList->previousInFreeList = freeListEnd->previousInFreeList = temp;
        temp->isFree= true;

    }

    else

    {

        temp->nextInFreeList = freeList;
        temp->previousInFreeList = freeListEnd;

        freeList->previousInFreeList =temp;

        freeListEnd->nextInFreeList = temp;

        freeList=temp;
         temp->isFree=true;
        cout<<". ";

    }

}

void Buffer::display()

{

    cout <<"Q0 :";displayQueue(0);
    cout <<"Q1 :";displayQueue(1);
    cout <<"Q2 :";displayQueue(2);
    cout <<"Q3 :";displayQueue(3);

    struct BufferBlock *s;

    if (freeList == freeListEnd && freeList == NULL)

    {

        cout<<"The List is empty, nothing to display"<<endl;

        return;

    }

    s = freeList;
    
   while(s!=freeListEnd)
   {
    
    cout<<" ["<<s->data<<","<<s->diskBlockNum<<","<<s->isFree<<"]";
    s=s->nextInFreeList;
   };

    cout<<" ["<<s->data<<","<<s->diskBlockNum<<","<<s->isFree<<"]"<<endl;

}

int Buffer::countFreeBuffer()
{ int count =0 ;
      

    struct BufferBlock *t;

    if (freeList == freeListEnd && freeList == NULL)

    {

        cout<<"The List is empty, nothing to display"<<endl;

        return 0;

    }

    t = freeList;
   while(t!=freeListEnd)
   {
    //cout<<t->data;
    count++;
    t=t->nextInFreeList;
   };
   count++;
   cout<<" ---\n";
    return count;

}

void Buffer::displayQueue(int queueNum)
{
  BufferBlock* temp;
  temp = hashQueue[queueNum];
  if(temp==NULL)
  {
    cout<<"hashQueue is empty\n";
  }
      else
      {
        while(temp->nextBlock!=NULL)
        {
             cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<"]";
             temp=temp->nextBlock;
        };
          cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<"]"<<endl;

      }
      return;

}

void Buffer::removeFromFreeList(BufferBlock* &freeBlock)
{
   if(freeList==freeListEnd)
      {
            cout<<"\nreached to freeblock check 2";
            freeBlock->nextInFreeList=NULL;
            freeBlock->previousInFreeList=NULL;
            freeList=NULL;
            freeListEnd=NULL;
            return;
                         }
  else if(freeListEnd==freeBlock)
                         {


                         cout<<"\n!!!";
                         freeListEnd = freeBlock->previousInFreeList;
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL; 
                         cout<<"removed from freeList\n";
                   }
  else if(freeList==freeBlock)
        {
                         cout<<"\n!!!";
                         freeList = freeBlock->nextInFreeList;
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL; 
                         cout<<"removed from freeList\n"; 


        } 
  else{
                  cout<<"\n!!!";
                        // freeListEnd = freeBlock->previousInFreeList;
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL; 
                         cout<<"removed from freeList\n";


        }

}

void Buffer::removeFromHashQueue(BufferBlock* &freeBlock)
{ if(freeBlock->nextBlock==NULL && freeBlock->previousBlock==NULL)
  {
    return;

  }
  
  else if(freeBlock->nextBlock!=NULL && freeBlock->previousBlock!=NULL)
                         {
                         freeBlock->previousBlock->nextBlock = freeBlock->nextBlock;
                         freeBlock->nextBlock->previousBlock = freeBlock->previousBlock;
                         freeBlock->previousBlock = NULL;
                         freeBlock->nextBlock = NULL ;
                         //bufferFound=true;
                         return;
                           }
  else if (freeBlock->previousBlock==NULL) //free block is first in hash queue
      {int qNum =  (freeBlock->diskBlockNum) %4;
            hashQueue[qNum]=freeBlock->nextBlock;
            //freeBlock->previousBlock->nextBlock = freeBlock->nextBlock;
            freeBlock->nextBlock->previousBlock = NULL;
            freeBlock->previousBlock = NULL;
            freeBlock->nextBlock = NULL ;
            //bufferFound=true;
            return;
      }
   else //freeBlock is last in hashQueue
   {   {int qNum =  (freeBlock->diskBlockNum) %4;
            hashQueue[qNum]=freeBlock->previousBlock;
            //freeBlock->previousBlock->nextBlock = freeBlock->nextBlock;
            freeBlock->previousBlock->nextBlock = NULL;
            freeBlock->previousBlock = NULL;
            freeBlock->nextBlock = NULL ;
            //bufferFound=true;
            return;

      }

   }    

}

bool Buffer :: searchInHq(int qNum,int diskBlkNum)
{ bool flag=false;
  BufferBlock* temp;
  temp = hashQueue[qNum];
  if(temp==NULL)
  {
    return false;
  }
      else
      {
        while(temp!=NULL)
        {
            if(temp->diskBlockNum==diskBlkNum)
            {
              return true;
            } 

             temp=temp->nextBlock;
        };
          //cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<"]"<<endl;

      }
      return false ;

}

BufferBlock* Buffer :: searchBlkInHq(int diskBlkNum)
{ bool flag=false;
  int qNum;
  qNum = diskBlkNum % 4;
  BufferBlock* temp;
  temp = hashQueue[qNum];
  if(temp==NULL)
  {
    return temp;
  }
      else
      {
        while(temp!=NULL)
        {
            if(temp->diskBlockNum==diskBlkNum)
            {
              return temp;
            } 

             temp=temp->nextBlock;
        };
          //cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<"]"<<endl;

      }
      return temp ;


}



 BufferBlock* Buffer :: getBlock(int blockNum)
                      {
                          int hashQueueNum = blockNum % HASHQNUM; // Get Hash queue
                          bool blockInHashQueue=false;
                          bool bufferLocked = true;
                          bool freeBlockAvailable=false;
                          bool bufferFound=false;
                          BufferBlock* temp; //Create a node
                          temp = hashQueue[hashQueueNum];
                          //cout<<"initialization done in get block \n";
                    while(!bufferFound)
                    { // cout<<"Entered Loop\n";
                            

                          if(freeList!=NULL)
                              {   //Check if Free Block Available
                                  freeBlockAvailable=true;
                              }
                           
                          if(temp!=NULL)
                          {   // displayQueue(hashQueueNum);
                              // cout<<" reached chkpt 4:\n"<<temp->diskBlockNum<<" "<<blockNum;
                               if(searchInHq(hashQueueNum,blockNum))
                               {
                                 for(;temp->diskBlockNum!=blockNum;temp=temp->nextBlock)
                                    {    //check if block is in buffer or not
            
                                        }
                                 if(temp->diskBlockNum==blockNum)
                                   {
                                     blockInHashQueue=true;
                                     if(temp->isFree==true)
                                        {
                                          bufferLocked=false;
                                         }
                                  }

                              
                                }

                          }
                         // cout<<"Initial checks performed  :"<<blockInHashQueue<<endl;
                         // cout<<"Block in hashqueue : "<<blockInHashQueue;
                     // case 1 block is in buffer and free
                     if(blockInHashQueue)
                     {
                         if(bufferLocked)
                         {
                             while(!temp->isFree)
                             {//event buffer becomes active
                             };
                             continue;
                         }
                       // cout<<"it was here ..";
                         //mark Buffer Busy
                         temp->isFree=false;
                         bufferFound=true;
                         //remove buffer from free List
                        // cout<<"block to be removed :"<<temp->data<<endl;
                         removeFromFreeList(temp);
                         /*
                         temp->previousInFreeList->nextInFreeList = temp ->nextInFreeList;
                         temp->nextInFreeList->previousInFreeList = temp ->previousInFreeList;
                         temp->nextInFreeList=NULL;
                         temp->previousInFreeList=NULL;
                         */
                         return temp;
                     }
                     else
                     {  //cout<<"freeBlockAvailable : "<<freeBlockAvailable<<endl;
                         if(!freeBlockAvailable)
                         {
                             while(freeList!=NULL)//Buffer becomes free
                             {

                             };
                             continue;
                         }
                        //cout<<"reached checkpoint 1 \n";
                         BufferBlock* freeBlock;
                         freeBlock= freeList;

                         //cout<<"reached checkpoint 1:";
                         //remove First from free list
                         removeFromFreeList(freeBlock);
                         

                         //scenario 3
                        // cout<<"\nreached to scenario 3:";
                         if(!freeBlock->dataWritten)
                         {
                             //write buffer to disk
                             bwrite(freeBlock);
                             continue;
                         }
                         //scenario 3
                         //remove Buffer from Old Hash Queue
                         removeFromHashQueue(freeBlock);
                         

                         bufferFound=true;

                         //Put Buffer in NewHashQueue
                         if (hashQueue[hashQueueNum] == NULL) //if hashqueue is empty
                         {
                             hashQueue[hashQueueNum] = freeBlock;
                             //freeBlock->diskBlockNum=blockNum;
                             freeBlock->isFree=false;

                         }
                         else
                         {   BufferBlock* tmp;    //if hashqueue is not empty
                             tmp = hashQueue[hashQueueNum];
                             while(tmp->nextBlock!=NULL)
                             {
                                 tmp=tmp->nextBlock;
                             };
                             tmp->nextBlock = freeBlock;
                             freeBlock->previousBlock=tmp;
                             //freeBlock->diskBlockNum=blockNum;
                             freeBlock->isFree=false;
                         }
                        
                       return freeBlock;
                     }



                      }

            } 



BufferBlock* Buffer :: bread(int blockRNum)
{  BufferBlock * block;
   block = getBlock(blockRNum);

   if(block->diskBlockNum==blockRNum)
        return block;

    else
    {
       block->diskBlockNum=blockRNum;
       block->data=Disk[blockRNum];    
       block->dataWritten = false; 
    }

   return block;
}

void Buffer :: bwrite(BufferBlock* block)
{
   Disk[block->diskBlockNum]=block->data;
   block->dataWritten= true;
   insertBlock(block);
   return;
  
} 

void Buffer :: actOnRequest( int &data,int &blk_Num,int &reqForm)
{  BufferBlock *temp;
   if (reqForm==0)
       { temp=bread(blk_Num);
         data = temp->data;
         blk_Num = temp->diskBlockNum;
         reqForm = 4;
       }
   else if(reqForm == 1)
   {    temp = searchBlkInHq(blk_Num);
       if(temp!=NULL)
       {
        
        insertBlock(temp);
        reqForm = 4;
       }
   }
   else if(reqForm == 2)
   {
      temp = searchBlkInHq(blk_Num);
       if(temp!=NULL)
       {
        temp->data = data;
        insertBlock(temp);
        reqForm = 4;
       }
   }
   else if(reqForm==3)
   {
    temp = searchBlkInHq(blk_Num);
    temp->data = data;
    bwrite(temp);
   }


}


int main()
{   cout<<"before declaring buffer";
    Buffer buf;
    char c;
    //BufferBlock* temp;
    //BufferBlock* temp2;
    cout<<"\n display buffer : \n";
    buf.display();
   
    cout<<"\nnumber of free blocks :"<<buf.countFreeBuffer();
  

int ctr =0;
int state=0;
int req =0;
bool otherProcessActive = true;

  int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd < 0) {
    perror("shm_open()");
    return EXIT_FAILURE;
  }
  

  ftruncate(fd, SIZE);

  struct Data *data = (struct Data *)
  mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("request: mapped address: %p\n", data);

  

  printf("request: release initial data\n");
  atomic_store(&data->state, 5);
while(otherProcessActive)
{ 
  printf("request: waiting updated data\n");
   //sleep(1);
  while (atomic_load(&data->state) < 6)
   {   //sleep(1);
        //ctr++;
        //if(ctr>15)
        //   otherProcessActive = false;
    
     };
 
  req= data->reqForm;
  buf.actOnRequest(data->data,data->blk_Num,data->reqForm);
   
   state =atomic_load(&data->state);
   if (req>0)
     atomic_store(&data->state, 5);
   else
     atomic_store(&data->state, state-6);
    //cout<<"ctr : "<<ctr<<endl;
    
   } ;

  munmap(data, SIZE);

  close(fd);

  shm_unlink(NAME);


    cout<<"\n display buffer after reading: \n"; 
    
    buf.display();

    cout<<"\n|||"<<freeList->data<<"|||"<<freeListEnd->data;
     return EXIT_SUCCESS;
 
}


