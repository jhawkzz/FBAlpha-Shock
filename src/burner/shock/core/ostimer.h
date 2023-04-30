
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

class OSTimer
{
public:
    OSTimer( );
    
    void   Reset( );
    UINT32 GetElapsedTimeMicroseconds( );
    
private:
    timespec Diff( timespec start, timespec end );
    timespec mZeroTime;
};

#endif
