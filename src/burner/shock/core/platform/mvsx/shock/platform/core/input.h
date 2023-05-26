
// See License.md for license

#ifndef INPUT_CORE_H_
#define INPUT_CORE_H_

#include "shock/systems.h"

// The path to the input device on the MVSX Linux OS. 
// This represents the entire control deck (both players, all buttons)
#define MVSX_INTEGRATED_DEVICE "/dev/input/event2"
#define INPUT_POLL_TIMEOUT  (300)

enum MVSXInput
{
    MVSXInput_P1_Green     = 304,
    MVSXInput_OptionsBack  = 305,
    
    MVSXInput_P1_Yellow    = 307,
    MVSXInput_P1_Start     = 308,
    MVSXInput_P1_RightBot  = 310,
    
    MVSXInput_P1_Blue      = 311,
    
    MVSXInput_P1_Joy_Left  = 312,
    MVSXInput_P1_Joy_Down  = 313,
    MVSXInput_P1_Joy_Right = 314,
    
    MVSXInput_P1_MidBot    = 315,
    MVSXInput_P1_Joy_Up    = 316,
    
    MVSXInput_P2_Start     = 320,
    MVSXInput_P2_Joy_Right = 321,
    MVSXInput_SelectGame   = 322,
    MVSXInput_P2_Joy_Left  = 323,
    MVSXInput_P2_Joy_Up    = 324,
    MVSXInput_P2_Joy_Down  = 325,
    MVSXInput_P2_RightBot  = 326,
    MVSXInput_P2_MidBot    = 327,
    MVSXInput_P1_Red       = 328,
    MVSXInput_P2_Red       = 329,
    MVSXInput_P2_Yellow    = 330,
    MVSXInput_P2_Blue      = 332,
    MVSXInput_P2_Green     = 333,
    
    MVSXInput_Count        = 334
};

struct MVSXInputState
{
    int             mutexCreated;
    pthread_mutex_t mutexLock;
    int             value; // This is the action (0 is released, 1 is down)
};

class InputCore
{
protected:
    static void CreateLookup( );
    static void *PollInput_ThreadProc( void *);
    static void  ReadInputs( );

    static MVSXInput      mMVSXInputLookup[ ShockButton_Count ];
    static MVSXInputState mMVSXInputState[ MVSXInput_Count ];
    static int            mInputFileHandle;
    static int            mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
