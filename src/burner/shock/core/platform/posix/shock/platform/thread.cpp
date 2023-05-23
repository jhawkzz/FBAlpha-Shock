
// See License.md for license

#include "shock/platform/thread.h"

ThreadImpl::~ThreadImpl()
{
    if ( mThread > -1 )
    {
        pthread_detach( mThread );
    }
}

int ThreadImpl::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &mThread, NULL, proc, param );
}
