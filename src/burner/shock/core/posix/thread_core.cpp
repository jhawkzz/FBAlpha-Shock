
// See License.md for license

#include "../../includes.h"

ShockThread::~ShockThread()
{
   pthread_detach( mThread );
}

int ShockThread::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &mThread, NULL, proc, param );
}
