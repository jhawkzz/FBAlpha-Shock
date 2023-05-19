
// See License.md for license

#ifndef OSTIMER_CORE_H_
#define OSTIMER_CORE_H_

class OSTimerCore
{
protected:
    timespec Diff( timespec start, timespec end );
    timespec mZeroTime;
};

#endif
