
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "shock/includes.h"
#include "shock/platform/ostimer.h"

class OSTimer : public OSTimerCore
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
};

#endif
