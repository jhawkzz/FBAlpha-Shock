
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "shock/platform/core/ostimer.h"
#include "burner.h"

class OSTimer : public OSTimerCore
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
};

#endif
