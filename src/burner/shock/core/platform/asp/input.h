
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

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

enum ShockButton
{
    P1_Joy_Up     = 0,
    P1_Joy_Left   = 1,
    P1_Joy_Right  = 2,
    P1_Joy_Down   = 3,
    
    P1_InsertCoin = 4,
    P1_Start      = 5,
    
    P1_Button_1   = 6,
    P1_Button_2   = 7,
    P1_Button_3   = 8,
    P1_Button_4   = 9,
    P1_Button_5   = 10,
    P1_Button_6   = 11,
    
    P2_Joy_Up     = 12,
    P2_Joy_Left   = 13,
    P2_Joy_Right  = 14,
    P2_Joy_Down   = 15,
    
    P2_InsertCoin = 16,
    P2_Start      = 17,
    
    P2_Button_1   = 18,
    P2_Button_2   = 19,
    P2_Button_3   = 20,
    P2_Button_4   = 21,
    P2_Button_5   = 22,
    P2_Button_6   = 23,
    
    ShockButton_Count = 24
};

struct ASPInputState
{
    int             mutexCreated;
    pthread_mutex_t mutexLock;
    int             value; // This is the action (0 is released, 1 is down)
};

class Input
{
public:
    static int  Create( );
    static void Destroy( );
    static int  GetValueForButton( ShockButton shockButton );
    
private:
    static void  CreateLookup( );
    static void *PollInput_ThreadProc( void *);
    static void  ReadInputs( );

    static ASPInput      mASPInputLookup[ ShockButton_Count ];
    static ASPInputState mASPInputState[ ASPInput_Count ];
    static int           mInputFileHandle;
    static int           mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
