#ifndef THREAD_H_
#define THREAD_H_

typedef void *(*ShockThreadProc)(void *);

#include "shock/platform/thread.h"

class Thread : public ThreadCore
{
public:
   ~Thread();

   int Create(ShockThreadProc, void* param);
};

#endif