#ifndef PROTOCOLP_H
#define PROTOCOLP_H

#include <atomic>




struct Data {
  std::atomic<int> state;
  int data;
  int blk_Num;
  int reqForm;
};

#define NAME "/area-shared6"

#define NUM 3

#define SIZE (sizeof(struct Data) + NUM * sizeof(int))

#endif  /* PROTOCOL_H */