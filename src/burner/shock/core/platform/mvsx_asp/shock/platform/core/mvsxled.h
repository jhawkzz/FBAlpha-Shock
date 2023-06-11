
// See License.md for license

#ifndef MVSX_LED_H_
#define MVSX_LED_H_

#define LED_PATH "/sys/class/led_board/led"

#include "shock/shock.h"

class MVSXLed
{
public:
    static int  Create( );
    static void Destroy( );
    
    static void SetLED( int playerIndex, int val );
    
    static int  GetLED( int playerIndex );
    
private:
    static void *UpdateLEDVals_ThreadProc( void *pArg );

    static int   mP1Led;
    static int   mP2Led;
    static int   mLedHandle;
    static int   mNeedsUpdate;
    static int   mThreadRunning; // No need for a mutex, we just use it to kill the thread
    
    static pthread_mutex_t mMutexLock;
    static int             mMutexCreated;
};

#endif
