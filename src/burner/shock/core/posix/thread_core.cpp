
// See License.md for license

#include "../../includes.h"

ShockThread::~ShockThread()
{
   pthread_detach( m_thread );
}

int ShockThread::Create(ShockThreadProc proc, void* param)
{
   return pthread_create( &m_thread, NULL, start_routine, restrict );
}
