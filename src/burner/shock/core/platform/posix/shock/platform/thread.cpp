
// See License.md for license

#include "shock/includes.h"
    
ShockThreadImpl::~ShockThreadImpl()
{
    if ( mThread > -1 )
    {
        pthread_detach( mThread );
    }
}

int ShockThreadImpl::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &mThread, NULL, proc, param );
}
