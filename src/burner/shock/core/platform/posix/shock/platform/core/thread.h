#ifndef THREAD_CORE_H_
#define THREAD_CORE_H_

#include "shock/systems.h"

class ThreadCore
{
public:
   ThreadCore()
      : mThread(-1) {}

   virtual ~ThreadCore() {}

   pthread_t mThread;
};

#endif