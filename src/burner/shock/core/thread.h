#ifndef THREAD_H_
#define THREAD_H_

typedef void *(*ShockThreadProc)(void *);

#include "thread_core.h"

class ShockThread : public ShockThreadCore
{
public:
   ~ShockThread();

   int Create(ShockThreadProc, void* param);
};

#endif