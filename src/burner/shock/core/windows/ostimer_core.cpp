
// See License.md for license

#include "../ostimer.h"

OSTimer::OSTimer( )
{
    __int64 seconds;

    QueryPerformanceFrequency( (LARGE_INTEGER *) &seconds );
    QueryPerformanceCounter( (LARGE_INTEGER *) &m_zero64 );

    //m_microsecond64 = seconds * 1000000;
    m_microsecond64 = seconds / 1000000;
}

void OSTimer::Reset( )
{   
    QueryPerformanceCounter( (LARGE_INTEGER *) &m_zero64 );
}

UINT32 OSTimer::GetElapsedTimeMicroseconds( )
{
    __int64 current64;

    QueryPerformanceCounter( (LARGE_INTEGER *) &current64 );

    return UINT32((current64 - m_zero64) / float(m_microsecond64));
}


