#ifndef THREAD_CORE_H_
#define THREAD_CORE_H_

#include "shock/systems.h"

class ThreadCore
{
public:
   virtual ~ThreadCore() {}

   pthread_t mThread = -1;
};

#endif