
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

#include "../includes.h"
#include "windows/ostimer_core.h"

class OSTimer : public OSTimerCore
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
};

#endif
