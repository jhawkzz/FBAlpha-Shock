
// See License.md for license

#include "shock/core/ostimer.h"

OSTimer::OSTimer( )
{
    memset( &mZeroTime, 0, sizeof( mZeroTime ) );
}

void OSTimer::Reset( )
{   
    clock_gettime( CLOCK_MONOTONIC, &mZeroTime );
}

UINT64 OSTimer::GetElapsedTimeMicroseconds( )
{
    timespec currTime;
    clock_gettime( CLOCK_MONOTONIC, &currTime );
    
    timespec delta = Diff( mZeroTime, currTime );
    
    UINT64 tvSec = (UINT64)delta.tv_sec;
    return (tvSec * SEC_TO_MICROSECONDS) + (delta.tv_nsec / NANO_TO_MICROSECONDS);
}

timespec OSTimerCore::Diff( timespec start, timespec end )
{
    timespec temp;
    if ( (end.tv_nsec-start.tv_nsec) < 0 ) 
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = SEC_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec;
    } 
    else 
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    
    return temp;
}
