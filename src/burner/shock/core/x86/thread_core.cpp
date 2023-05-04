
// See License.md for license

#include "../../includes.h"

int ShockThreadCreate(ShockThread* p, void *(*start_routine)(void *), void* restrict)
{
   return pthread_create( &p->t, NULL, start_routine, restrict );
}


void ShockThreadDetach(ShockThread* p)
{
   pthread_detach( p->t );
}