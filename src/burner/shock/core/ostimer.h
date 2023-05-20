
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "ostimer_impl.h"
#include "../includes.h"

class OSTimer : public OSTimerImpl
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
};

#endif
