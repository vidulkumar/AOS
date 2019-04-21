
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>
#include <queue> 
#include <csignal>


#define HASHQNUM 4
#define BUFBLOCKNUM 4
#define DISKSIZE 50
#define MSZ 5

int d=0;


typedef struct Data {
  long to;
  long from;
  int data[MSZ];

} MESSAGE;
 
int mid;
key_t key;
struct msqid_ds buf;
MESSAGE msg;

using namespace std;



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

                    int *Disk;

                  public:

                     Buffer();
                     void createDisk();
                     void insertBlock(BufferBlock* &temp,int mod);
                     void createBufferBlock();
                     void display();
                     int countFreeBuffer();
                     void removeFromFreeList(BufferBlock* &freeBlock);
                     void removeFromHashQueue(BufferBlock* &freeBlock);
                     void displayQueue(int queueNum);
                     BufferBlock* bread(int blockRNum);
                     void  bwrite(BufferBlock* block,int mod);
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
    insertBlock(tempBlock,1);
    return ;

}                      


void Buffer::insertBlock( BufferBlock* &temp,int mod=1)

{

   // cout<<"executing insertBlock\n";
 

    if (freeList == freeListEnd && freeList == NULL)

    {    

        cout<<"||";

        freeList = freeListEnd = temp;

        freeList->nextInFreeList = freeListEnd->nextInFreeList =temp;

        freeList->previousInFreeList = freeListEnd->previousInFreeList = temp;
        temp->isFree= true;

    }

    else if(mod==1)

    {

        temp->nextInFreeList = freeList;
        temp->previousInFreeList = freeListEnd;

        freeList->previousInFreeList =temp;

        freeListEnd->nextInFreeList = temp;

        freeListEnd=temp;
        temp->isFree=true;
        cout<<". ";

    }
    else  { // when mod is 2
         //cout<<"executing case 2";
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
     cout<<"\n=========BUFFER====[data,block_num,free]=============\n";
    cout <<"Q0 :";displayQueue(0);
    cout <<"Q1 :";displayQueue(1);
    cout <<"Q2 :";displayQueue(2);
    cout <<"Q3 :";displayQueue(3);
    cout <<"FL :";
    struct BufferBlock *s;

    if (freeList == freeListEnd && freeList == NULL)

    {

        cout<<"The List is empty, nothing to display"<<endl;

        return;

    }

    s = freeList;
    
   while(s!=freeListEnd)
   {
    
    cout<<" ["<<s->data<<","<<s->diskBlockNum<<","<<s->isFree<<","<<s->dataWritten<<"]";
    s=s->nextInFreeList;
    //sleep(1);
   };

    cout<<" ["<<s->data<<","<<s->diskBlockNum<<","<<s->isFree<<","<<s->dataWritten<<"]"<<endl;
    cout<<"========================================================\n";
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
             cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<","<<temp->dataWritten<<"]";
             temp=temp->nextBlock;
             sleep(1);
        };
           cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<","<<temp->dataWritten<<"]"<<endl;

      }
      return;

}

void Buffer::removeFromFreeList(BufferBlock* &freeBlock)
{
   if(freeList==freeListEnd)
      {
            //cout<<"\nreached to freeblock check 2";
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
                         //cout<<"removed from freeList1\n";
                   }
  else if(freeList==freeBlock)
        {
                         cout<<"\n!!!";
                         freeList = freeBlock->nextInFreeList;
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL; 
                         //cout<<"removed from freeList2\n"; 


        } 
  else{
                  cout<<"\n!!!";
                        // freeListEnd = freeBlock->previousInFreeList;
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL; 
                         //cout<<"removed from freeList3\n";


        }

}

void Buffer::removeFromHashQueue(BufferBlock* &freeBlock)
{ 
  int num = freeBlock->diskBlockNum%4;
  
   if (num==-1)
    return;
   
  if(freeBlock->nextBlock==NULL && freeBlock->previousBlock==NULL)
  { 
     hashQueue[num]=NULL;
    //cout<<"removeFromHashQueue.. "<<freeBlock->diskBlockNum;
    return;

  }
  
  else if(freeBlock->nextBlock!=NULL && freeBlock->previousBlock!=NULL)
                         {
                         freeBlock->previousBlock->nextBlock = freeBlock->nextBlock;
                         freeBlock->nextBlock->previousBlock = freeBlock->previousBlock;
                         freeBlock->previousBlock = NULL;
                         freeBlock->nextBlock = NULL ;
                         //bufferFound=true;
                         cout<<"removedFromHashQueue..";
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
            cout<<"removeFromHashQueue..";
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
            cout<<"removeFromHashQueue..";
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
                          BufferBlock* temp2 = NULL;
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
                          //cout<<"Initial checks performed  :"<<blockInHashQueue<<endl;
                          cout<<"Block in hashqueue : "<<blockInHashQueue<<endl;
                        //case 1 block is in buffer and free
                     if(blockInHashQueue)
                     {
                         if(bufferLocked)
                         {  return temp2;
                            /* while(!temp->isFree)
                             {//event buffer becomes active
                             };
                             continue; */
                         }
                       // cout<<"it was here ..";
                         //mark Buffer Busy
                         temp->isFree=false;
                         bufferFound=true;
                         //remove buffer from free List
                        //cout<<"block to be removed0 :"<<temp->data<<endl;
                         cout<<"lock this block\n";
                         removeFromFreeList(temp);

                         return temp;
                     }
                     else
                     {   cout<<"freeBlockAvailable : "<<freeBlockAvailable<<endl;
                         if(!freeBlockAvailable)
                         {  return temp2;
                         /*
                             while(freeList!=NULL)//Buffer becomes free
                             {

                             };
                             continue; */
                         }
                        //cout<<"reached checkpoint 1 \n";
                         BufferBlock* freeBlock;
                         freeBlock= freeList;

                         //cout<<"reached checkpoint 1:";
                         //remove First from free list

                         removeFromFreeList(freeBlock);
                         cout<<"remove a block from free list\n";
                         

                         //scenario 3
                        // cout<<"\nreached to scenario 3:";
                         //cout<<"dataWritten: "<<freeBlock->dataWritten<<endl;
                         if(freeBlock->dataWritten != true)
                         {   
                             //write buffer to disk
                              cout<<"delayed write case handled \n";
                              temp = freeList;
                            if(temp->dataWritten==true && temp!=NULL) // if first was delaywrite
                                {
                                       bwrite(freeBlock,2);
                                       system("gnome-terminal --command='./asy'");
                                       sleep(2);
                                       freeBlock = temp;
                                       removeFromFreeList(freeBlock);
                                   }
 
                             else
                               {
                                  for(temp=freeList->nextInFreeList;temp!=freeList && temp->dataWritten!=true && temp!=NULL;temp=temp->nextInFreeList)
                                    { }

                             cout<<"after executing delayed write case --";
                             cout<<" ["<<temp->data<<","<<temp->diskBlockNum<<","<<temp->isFree<<","<<temp->dataWritten<<"]"<<endl;
                             //display();
                            if(temp!=freeList && temp!=NULL)
                            { bwrite(freeBlock,2);
                              system("gnome-terminal --command='./asy'");
                              sleep(2);
                              freeBlock = temp;
                              removeFromFreeList(freeBlock);
                            } 
                            else
                            {  
                              
                              system("gnome-terminal --command='./asy'");
                              bwrite(freeBlock,2);
                              continue; }      

                               }
                             

                         }
                         //scenario 3
                         //remove Buffer from Old Hash Queue

                         removeFromHashQueue(freeBlock);
                         //cout<<"removed from old hashqueue\n";

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
                        cout<<"added to new hash queue \n";
                       return freeBlock;
                     }

                      }

            } 



BufferBlock* Buffer :: bread(int blockRNum)
{  BufferBlock * block;
   block = getBlock(blockRNum);
  if (block==NULL)
     return block;

   if(block->diskBlockNum==blockRNum)
        return block;

    else
    {  sleep(2);
       block->diskBlockNum=blockRNum;
       block->data=Disk[blockRNum];    
       block->dataWritten = true; 
    }

   return block;
}

void Buffer :: bwrite(BufferBlock* block,int mod=1)
{  //cout<<"Entered write ";
   sleep(2);
   Disk[block->diskBlockNum]=block->data; 
   block->dataWritten= true;
   if(mod==2)
   {
    insertBlock(block,2);
    return;
   }
   else{
   insertBlock(block);
   return; }
  
} 

void Buffer :: actOnRequest( int &data,int &blk_Num,int &reqForm)
{  BufferBlock *temp;
   if (reqForm==0)   // read block
       { temp=bread(blk_Num);
         if(temp==NULL)
         { reqForm = -1;}
        else
        { data = temp->data;
         //blk_Num = temp->diskBlockNum;
         reqForm = 1;
        }
         //cout<<"\nread successful";
       }
   else if(reqForm == 1) // 
   {    temp = searchBlkInHq(blk_Num);
       if(temp!=NULL)
       {
         //cout<<"\nEntered set Free----\n";
         //temp->dataWritten=true;
        insertBlock(temp);
        //cout<<"\nset free successful";
        //reqForm = 4;
       }
   }
   else if(reqForm == 2)
   {
      temp = searchBlkInHq(blk_Num);
       if(temp!=NULL)
       {
        temp->data = data;
        temp->dataWritten=false;
        insertBlock(temp);

        //cout<<"\nupdate and set free successful";
        //reqForm = 4;
       }
   }
   else if(reqForm==3)
   {
    temp = searchBlkInHq(blk_Num);
    temp->data = data;
    temp->dataWritten=false;
    bwrite(temp);

        //cout<<"\nwrite set free successful";
   }
   else if(reqForm==4)
   { //cout<<"reached display\n";
     display();

        //cout<<"\ndisplay successful";
   }


}


int main()
{   queue<long> waitQueue;
    Buffer buf;
    char c;
    int data;
    int blk_Num;
    int reqForm;
     long client ;
     int rf;
    const int MYPID = getpid();
    cout<<"\n display buffer : \n";
    buf.display();
   
    cout<<"\nnumber of free blocks :"<<buf.countFreeBuffer()<<endl;
   //Creating a message queue
    key = getuid();
    
    if((mid = msgget(key, IPC_CREAT | 0660))<0){
        printf("Error Creating Message Queue\n");
        exit(-1);
    }
 
    //Display Message Queue and Server ID
    printf("Message Queue ID: %d\n", mid);
    printf("Server ID: %ld\n", (long)getpid());    
    while(1)
    {
    //Receiving message from client, throws and error if input is invalid
    cout<<"Waiting for request ......\n";
    if(msgrcv(mid, &msg, sizeof(msg.data), MYPID, 0)<0){
        perror("msgrcv");
        exit(-1);
    }
  client = msg.from;  
  data=msg.data[0];
  blk_Num=msg.data[1];
  reqForm=msg.data[2];
  if(reqForm==6)
           {exit(0);}
  cout<<"["<<msg.to<<","<<msg.from<<","<<data<<","<<blk_Num<<","<<reqForm<<"]";
  rf=reqForm;
  buf.actOnRequest(data,blk_Num,reqForm);
  buf.display();
  if(rf==0)
  {
           msg.from = MYPID;
           msg.to = client;
           msg.data[0]=data;
           msg.data[1]=blk_Num;
           msg.data[2]=reqForm;
           
           cout<<"["<<msg.to<<","<<msg.from<<","<<data<<","<<blk_Num<<","<<reqForm<<"]";
           if(msgsnd(mid, ( MESSAGE*)&msg, sizeof(msg.data), 0)==-1){
              perror("msgsnd");
              exit(-1);
              }
           if(reqForm==-1)
             {
                waitQueue.push(client);
             }  
  }
  else if(rf==1 || rf==2 || rf == 3)
  {
           while (!waitQueue.empty()) { 
           kill(waitQueue.front(),SIGUSR1); 
           waitQueue.pop(); 
           }
  }
  


    };
    
    cout<<"\n display buffer after reading: \n"; 
    
    buf.display();

    cout<<"\n|||"<<freeList->data<<"|||"<<freeListEnd->data;
     return EXIT_SUCCESS;
 

}

