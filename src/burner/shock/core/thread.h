#ifndef THREAD_H_
#define THREAD_H_

#include "thread_core.h"

class ShockThread : public ShockThreadCore {};

int ShockThreadCreate(ShockThread* p, void *(*start_routine)(void *), void* restrict);
void ShockThreadDetach(ShockThread* p);

#endif