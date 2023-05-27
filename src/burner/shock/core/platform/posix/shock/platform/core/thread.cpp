
// See License.md for license

#include "shock/core/thread.h"

Thread::~Thread()
{
    if ( mThread > -1 )
    {
        pthread_detach( mThread );
    }
}

int Thread::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &mThread, NULL, proc, param );
}
