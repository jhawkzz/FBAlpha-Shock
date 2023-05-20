
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "shock/includes.h"
#include "ostimer_impl.h"

class OSTimer : public OSTimerImpl
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
};

#endif
