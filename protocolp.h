#ifndef PROTOCOLP_H
#define PROTOCOLP_H

#include <atomic>
/*
0 - proc 0 locked it
1 - proc 1 locked it
2 - proc 2 locked it
3 - proc 3 locked it
4 - available to all child process
5 - available to getblock algo with request
6 - reply to read req proc 
*/
struct Data {
  std::atomic<int> state;
  int data;
  int blk_Num;
  int reqForm;
};

#define NAME "/area-shared1"

#define NUM 3

#define SIZE (sizeof(struct Data) + NUM * sizeof(int))

#endif  /* PROTOCOL_H */