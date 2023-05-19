
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

// Keyboard device
#define KB_LOCAL_DEVICE     "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define INPUT_POLL_TIMEOUT  (300)

enum InputCodeToButtonMapping
{
    P1_Start     = 2, // Keyboard 1
    OptionsBack  = 6, // Keyboard 5
    SelectGame   = 7, // Keyboard 6
    P2_Start     = 3, // Keyboard 2
    
    P2_Yellow    = 21, // Keyboard Y
    P2_Green     = 22, // Keyboard U
    P2_Blue      = 23, // Keyboard I
    
    P1_Yellow    = 30, // Keyboard A
    P1_Green     = 31, // Keyboard S
    P1_Blue      = 32, // Keyboard D
    
    P2_Red       = 35, // Keyboard H
    P2_MidBot    = 36, // Keyboard J
    P2_RightBot  = 37, // Keyboard K
    
    P1_Red       = 44, // Keyboard Z
    P1_MidBot    = 45, // Keyboard X
    P1_RightBot  = 46, // Keyboard C
    
    P1_Joy_Up    = 103, // Keyboard Up Arrow
    P2_Joy_Up    = 102, // Keyboard Home
    P1_Joy_Right = 106, // Keyboard Right Arrow
    P2_Joy_Down  = 107, // Keyboard End
    P1_Joy_Down  = 108, // Keyboard Down Arrow
    P1_Joy_Left  = 105, // Keyboard Left Arrow
    P2_Joy_Right = 109, // Keyboard PgDn
    P2_Joy_Left  = 111, // Keyboard Delete
    
    Button_Count
};

struct ButtonState
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
    static int  GetValueForInput( InputCodeToButtonMapping input );
    
private:
    static void *PollInput_ThreadProc( void *);
    static void  ReadInputs( );

    static ButtonState mButtonState[ Button_Count ];
    static int         mInputFileHandle;
    static int         mThreadRunning; // No need for a mutex, we just use it to kill the thread
};

#endif
