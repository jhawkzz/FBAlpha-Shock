
// See License.md for license

#ifndef INPUT_CORE_H_
#define INPUT_CORE_H_

#include "shock/systems.h"

// The path to the input device on the MVSX Linux OS. 
// This represents the entire control deck (both players, all buttons)
#define MVSX_INTEGRATED_DEVICE "/dev/input/event2"

// The path to the input device on the ASP Linux OS. 
#define ASP_INTEGRATED_DEVICE "/dev/input/event1"

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

enum ASPInput
{
    ASPInput_P1_Joy_Up    = 103,
    ASPInput_P1_Joy_Left  = 105,
    ASPInput_P1_Joy_Right = 106,
    ASPInput_P1_Joy_Down  = 108,

    ASPInput_Options      = 278,
    ASPInput_Turbo        = 279,

    ASPInput_RedA         = 304,
    ASPInput_YellowB      = 305,
    ASPInput_GreenC       = 306,
    ASPInput_TopGray      = 307,
    ASPInput_BotGray      = 308,
    ASPInput_BlueD        = 309,
    ASPInput_TopWhite     = 310,
    ASPInput_BotWhite     = 311,
    
    ASPInput_Select       = 314,
    ASPInput_Start        = 315,
    
    ASPInput_Count        = 316
};

struct InputCoreState
{
    int             mutexCreated;
    pthread_mutex_t mutexLock;
    int             value; // This is the action (0 is released, 1 is down)
};

class InputCore
{
protected:
    static void  CreateLookup_MVSX( );
    static void  ReadInputs_MVSX( );
    static int   GetValueForButton_MVSX( ShockButton shockButton );

    static void  CreateLookup_ASP( );
    static void  ReadInputs_ASP( );
    static int   GetValueForButton_ASP( ShockButton shockButton );

    static void *PollInput_ThreadProc( void * );

    static MVSXInput      mMVSXInputLookup[ ShockButton_Count ];
    static InputCoreState mMVSXInputState[ MVSXInput_Count ];

    static ASPInput       mASPInputLookup[ ShockButton_Count ];
    static InputCoreState mASPInputState[ ASPInput_Count ];

    static int            mInputFileHandle;
    static int            mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
