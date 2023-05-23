
// See License.md for license

#include "shock/includes.h"

ShockThread::ShockThread()
{
    mThread = -1;
}

ShockThread::~ShockThread()
{
    if( mThread > -1 )
    {
        pthread_detach( mThread );
    }
}

int ShockThread::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &mThread, NULL, proc, param );
}
