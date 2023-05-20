#ifndef THREAD_H_
#define THREAD_H_

typedef void *(*ShockThreadProc)(void *);

#include "thread_impl.h"

class ShockThread : public ShockThreadImpl
{
public:
    ShockThread();
   ~ShockThread();

   int Create(ShockThreadProc, void* param);
};

#endif