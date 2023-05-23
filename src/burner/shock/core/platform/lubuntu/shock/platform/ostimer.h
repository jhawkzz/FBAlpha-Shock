
// See License.md for license

#ifndef OSTIMER_IMPL_H_
#define OSTIMER_IMPL_H_

class OSTimerImpl
{
protected:
    timespec Diff( timespec start, timespec end );
    timespec mZeroTime;
};

#endif
