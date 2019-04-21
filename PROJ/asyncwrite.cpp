#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
   cout<<"writing asynchronously....";
   fflush(NULL);
   sleep(2);
   return 0;
}