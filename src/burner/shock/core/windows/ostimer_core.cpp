
// See License.md for license

#include "../ostimer.h"

OSTimer::OSTimer( )
{
    __int64 seconds;

    QueryPerformanceFrequency( (LARGE_INTEGER *) &seconds );
    QueryPerformanceCounter( (LARGE_INTEGER *) &m_zero64 );

    m_microsecond64 = seconds / 1000000;
}

void OSTimer::Reset( )
{   
    QueryPerformanceCounter( (LARGE_INTEGER *) &m_zero64 );
}

UINT32 OSTimer::GetElapsedTimeMicroseconds( )
{
    __int64 current64;

    //Where is the cpu in the second?
    QueryPerformanceCounter( (LARGE_INTEGER *) &current64 );

    //_control87( _PC_53, _MCW_PC );

    return (current64 - m_zero64) / m_microsecond64;

    //m_total = static_cast <float> ( m_total64 ) / static_cast <float> ( m_second64 );

    //m_delta64 = m_total64 - m_last64; 

    //m_delta = static_cast <float> ( m_delta64 );

    //m_rate = (float) m_second64 / m_delta64;

    //m_last64 = m_total64;

    //_control87( _PC_24, _MCW_PC );

    return S_OK;
}


