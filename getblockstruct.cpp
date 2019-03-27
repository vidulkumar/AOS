#include <iostream>
#define HASHQNUM 4
#define BUFBLOCKNUM 20
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
                        };

class Buffer {
                      BufferBlock* hashQueue[HASHQNUM];
                      BufferBlock* freeList;

                  public:

                      Buffer();
                      BufferBlock* getBlock(int blockNum);



};

 Buffer :: Buffer()
                      {   freeList = NULL;
                          for(int i=0;i<HASHQNUM;i++)
                          {
                              hashQueue[i] = NULL;
                          }
                          //Adding First Node to the FreeList
                              BufferBlock *tempBlock = new BufferBlock;
                              tempBlock->diskBlockNum       = -1;
                              tempBlock->isFree             = true;
                              tempBlock->dataWritten        = true;
                              tempBlock->nextBlock          = NULL;
                              tempBlock->previousBlock      = NULL;
                              tempBlock->nextInFreeList     = freeList;
                              tempBlock->previousInFreeList = freeList;


                          for(int i=0;i<BUFBLOCKNUM;i++)
                          {
                              BufferBlock* newBlock = new BufferBlock; //Create a node
                              newBlock->diskBlockNum       = -1;
                              newBlock->isFree             = true;
                              newBlock->dataWritten        = true;
                              newBlock->nextBlock          = NULL;
                              newBlock->previousBlock      = NULL;
                              newBlock->nextInFreeList     = NULL;
                              newBlock->previousInFreeList = NULL;

                              newBlock->nextInFreeList=freeList; // AddNode to Free List
                              freeList->previousInFreeList->nextInFreeList= newBlock;
                              newBlock->previousInFreeList=freeList->previousInFreeList;
                              freeList->previousInFreeList=newBlock;
                              freeList = newBlock;


                          }

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

                    while(!bufferFound)
                    {  cout<<"Entered Loop";


                          if(freeList!=NULL)
                              {   //Check if Free Block Available
                                  freeBlockAvailable=true;
                              }

                          if(temp!=NULL)
                          {

                          while(temp->diskBlockNum!=blockNum||temp->nextBlock!=NULL)
                          {
                            temp = temp -> nextBlock;


                          };

                          if(temp->diskBlockNum==blockNum)
                          {   //check if block is in buffer or not
                              blockInHashQueue = true;
                              if(temp->isFree==true)
                                {   //Check if  Block Locked or Free
                                    bufferLocked = false;
                                }

                          }


                          }

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

                         //mark Buffer Busy
                         temp->isFree=false;
                         bufferFound=true;
                         //remove buffer from free List

                         temp->previousInFreeList->nextInFreeList = temp ->nextInFreeList;
                         temp->nextInFreeList->previousInFreeList = temp ->previousInFreeList;
                         temp->nextInFreeList=NULL;
                         temp->previousInFreeList=NULL;

                         return temp;
                     }
                     else
                     {
                         if(!freeBlockAvailable)
                         {
                             while(freeList!=NULL)//Buffer becomes free
                             {

                             };
                             continue;
                         }

                         BufferBlock* freeBlock;
                         freeBlock=freeList;
                         //remove First from free list
                         if(freeBlock->nextInFreeList!=freeBlock)
                         {

                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL;
                         }
                         else
                         {
                         freeBlock->previousInFreeList->nextInFreeList = freeBlock ->nextInFreeList;
                         freeBlock->nextInFreeList->previousInFreeList = freeBlock ->previousInFreeList;
                         freeBlock->nextInFreeList=NULL;
                         freeBlock->previousInFreeList=NULL;
                         freeList=NULL;
                         }

                         //scenario 3
                         if(!freeBlock->dataWritten)
                         {
                             //write buffer to disk
                             continue;
                         }
                         //scenario 3
                         //remove Buffer from Old Hash Queue
                         freeBlock->previousBlock->nextBlock = freeBlock->nextBlock;
                         freeBlock->nextBlock->previousBlock = freeBlock->previousBlock;
                         freeBlock->previousBlock = NULL;
                         freeBlock->nextBlock = NULL ;
                         bufferFound=true;

                         //Put Buffer in NewHashQueue
                         if (hashQueue[hashQueueNum] == NULL) //if hashqueue is empty
                         {
                             hashQueue[hashQueueNum] = freeBlock;
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
                         }

                       return freeBlock;
                     }



                      }

            }


int main()
{   Buffer buf;
    BufferBlock* block;
    int BNum;
    cout<<"Enter Block Number Required : ";
    cin>>BNum;
    block =buf.getBlock(2);


}


