#ifndef THREAD_H_
#define THREAD_H_

typedef void *(*ShockThreadProc)(void *);

#include "shock/platform/thread.h"

class Thread : public ThreadImpl
{
public:
    Thread();
   ~Thread();

   int Create(ShockThreadProc, void* param);
};

#endif