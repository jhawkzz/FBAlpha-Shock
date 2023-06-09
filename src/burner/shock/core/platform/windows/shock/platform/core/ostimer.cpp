
// See License.md for license

#include "shock/core/ostimer.h"

OSTimer::OSTimer( )
{
    __int64 seconds;

    QueryPerformanceFrequency( (LARGE_INTEGER *) &seconds );
    QueryPerformanceCounter( (LARGE_INTEGER *) &mZero64 );

    mMicrosecond64 = seconds / 1000000;
}

void OSTimer::Reset( )
{   
    QueryPerformanceCounter( (LARGE_INTEGER *) &mZero64 );
}

UINT64 OSTimer::GetElapsedTimeMicroseconds( )
{
    __int64 current64;

    QueryPerformanceCounter( (LARGE_INTEGER *) &current64 );

    return UINT64((current64 - mZero64) / float(mMicrosecond64));
}


