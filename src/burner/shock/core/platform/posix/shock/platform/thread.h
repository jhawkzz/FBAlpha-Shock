#ifndef THREAD_IMPL_H_
#define THREAD_IMPL_H_

#include "shock/systems.h"

class ThreadImpl
{
public:
   virtual ~ThreadImpl();

   pthread_t mThread = -1;
};

#endif