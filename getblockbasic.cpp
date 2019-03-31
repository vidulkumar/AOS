
#include <iostream>
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
                      
                      BufferBlock* getBlock(int blockNum);

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



 BufferBlock* Buffer :: getBlock(int blockNum)
                      {
                          int hashQueueNum = blockNum % HASHQNUM; // Get Hash queue
                          bool blockInHashQueue=false;
                          bool bufferLocked = true;
                          bool freeBlockAvailable=false;
                          bool bufferFound=false;
                          BufferBlock* temp; //Create a node
                          temp = hashQueue[hashQueueNum];
                          cout<<"initialization done in get block \n";
                    while(!bufferFound)
                    {  cout<<"Entered Loop\n";
                            

                          if(freeList!=NULL)
                              {   //Check if Free Block Available
                                  freeBlockAvailable=true;
                              }
                           
                          if(temp!=NULL)
                          {   // displayQueue(hashQueueNum);
                               cout<<" reached chkpt 4:\n"<<temp->diskBlockNum<<" "<<blockNum;
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
                          cout<<"Initial checks performed  :"<<blockInHashQueue<<endl;
                          cout<<"Block in hashqueue : "<<blockInHashQueue;
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
                        cout<<"it was here ..";
                         //mark Buffer Busy
                         temp->isFree=false;
                         bufferFound=true;
                         //remove buffer from free List
                         cout<<"block to be removed :"<<temp->data<<endl;
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
                     {  cout<<"freeBlockAvailable : "<<freeBlockAvailable<<endl;
                         if(!freeBlockAvailable)
                         {
                             while(freeList!=NULL)//Buffer becomes free
                             {

                             };
                             continue;
                         }
                        cout<<"reached checkpoint 1 \n";
                         BufferBlock* freeBlock;
                         freeBlock= freeList;

                         //cout<<"reached checkpoint 1:";
                         //remove First from free list
                         removeFromFreeList(freeBlock);
                         

                         //scenario 3
                         cout<<"\nreached to scenario 3:";
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

int main()
{   cout<<"before declaring buffer";
    Buffer buf;
    BufferBlock* temp;
    BufferBlock* temp2;
    //cout<<"\nnumber of free blocks :"<<buf.countFreeBuffer();
    cout<<"\n display buffer : \n";
   

    buf.display();
   // buf.display();
    cout<<"\nnumber of free blocks :"<<buf.countFreeBuffer();
    //buf.InitializeBuffer();
    //BufferBlock* block;
    int BNum;
   
    cout<<"\n|||"<<freeList->data<<"|||"<<freeListEnd->data;
   
    for(int i = 0; i<45; i=i+3) 
       {  // buf.getBlock(i);
       //buf.bread(i);

       if(i<5)
        temp = buf.bread(i);
       else
        if(i==21)
        temp2 = buf.bread(i);
        else
        buf.bread(i);       
             //cout<<"\n display buffer : \n";
    //buf.display();      
        }
  cout<<"\n display buffer before writing: \n";
    buf.display();
       temp->data = 23;
        buf.bwrite(temp);
       temp2->data = 56;
        buf.bwrite(temp2);
   // cout<<"\n display buffer : \n";
   // buf.display();

   cout<<"diskblk 12 in hq :"<<buf.searchInHq(0,12);
    cout<<"\n display buffer after writing: \n";
    buf.display();

    buf.bread(21);
    cout<<"\n display buffer after reading: \n";
    buf.display();
    
    cout<<"\n|||"<<freeList->data<<"|||"<<freeListEnd->data;

}


