
// See License.md for license

#ifndef INPUT_CORE_H_
#define INPUT_CORE_H_

#include "shock/shock.h"

// The path to the input device on the ASP Linux OS. 
#define ASP_INTEGRATED_DEVICE "/dev/input/event1"
#define INPUT_POLL_TIMEOUT  (300)

enum ASPInput
{
    ASPInput_P1_Joy_Up    = 103,
    ASPInput_P1_Joy_Left  = 105,
    ASPInput_P1_Joy_Right = 106,
    ASPInput_P1_Joy_Down  = 108,
    
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
    ASPInput_Options      = 278,
    ASPInput_Turbo        = 379,
    
    ASPInput_Count        = 380
};

struct ASPInputState
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

    static ASPInput      mASPInputLookup[ ShockButton_Count ];
    static ASPInputState mASPInputState[ ASPInput_Count ];
    static int           mInputFileHandle;
    static int           mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
