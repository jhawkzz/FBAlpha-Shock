
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "shock/shock.h"
#include "shock/platform/core/ostimer.h"

class OSTimer : public OSTimerCore
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT64 GetElapsedTimeMicroseconds( );
};

#endif
