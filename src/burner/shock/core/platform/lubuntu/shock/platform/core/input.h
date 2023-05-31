
// See License.md for license

#ifndef INPUT_CORE_H_
#define INPUT_CORE_H_

#include "shock/shock.h"

// Keyboard device
#define KB_LOCAL_DEVICE     "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define INPUT_POLL_TIMEOUT  (300)

enum LubuntuInput
{
    LubuntuInput_KB_1      = 2,
    LubuntuInput_KB_2      = 3,
    LubuntuInput_KB_5      = 6,
    LubuntuInput_KB_6      = 7,
    
    LubuntuInput_KB_Y      = 21,
    LubuntuInput_KB_U      = 22,
    LubuntuInput_KB_I      = 23,
    
    LubuntuInput_KB_A      = 30,
    LubuntuInput_KB_S      = 31,
    LubuntuInput_KB_D      = 32,
    
    LubuntuInput_KB_H      = 35,
    LubuntuInput_KB_J      = 36,
    LubuntuInput_KB_K      = 37,
    
    LubuntuInput_KB_Z      = 44,
    LubuntuInput_KB_X      = 45,
    LubuntuInput_KB_C      = 46,
    
    LubuntuInput_KB_Home   = 102,
    LubuntuInput_KB_Up     = 103,
    LubuntuInput_KB_Left   = 105,
    LubuntuInput_KB_Right  = 106,
    LubuntuInput_KB_End    = 107,
    LubuntuInput_KB_Down   = 108,
    LubuntuInput_KB_PgDn   = 109,
    LubuntuInput_KB_Delete = 111,
    
    LubuntuInput_Count     = 112
};

struct LubuntuInputState
{
    int             mutexCreated;
    pthread_mutex_t mutexLock;
    int             value; // This is the action (0 is released, 1 is down)
};

class InputCore
{
protected:
    static void  CreateLookup( );
    static void *PollInput_ThreadProc( void *);
    static void  ReadInputs( );

    static LubuntuInput      mLubuntuInputLookup[ ShockButton_Count ];
    static LubuntuInputState mLubuntuInputState[ LubuntuInput_Count ];
    static int               mInputFileHandle;
    static int               mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
