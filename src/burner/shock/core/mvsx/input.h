
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

// The path to the input device on the MVSX Linux OS. 
// This represents the entire control deck (both players, all buttons)
#define SNK_JS_LOCAL_DEVICE "/dev/input/event2"
#define INPUT_POLL_TIMEOUT  (300)

/*Mappings
Value 0: Released
Value 1: Pressed

T: Is always 1

Weirdly these are not sequential at all. (there is no 309, for example)

316 = P1 Joy Up
312 = P1 Joy Left
313 = P1 Joy Down
314 = P1 Joy Right
308 = P1 Start
328 = P1 Red
307 = P1 Yellow
304 = P1 Green
311 = P1 Blue
315 = P1 Inner Black (Orig White, middle button bottom row)
310 = P1 Outer Black (Orig White, outer button bottom row)

305 = Options/Back
322 = Select Game

324 = P2 Joy Up
323 = P2 Joy Left
325 = P2 Joy Down
321 = P2 Joy Right
320 = P2 Start
329 = P2 Red
330 = P2 Yellow
333 = P2 Green
332 = P2 Blue
327 = P2 Inner Black (Orig White, middle button bottom row)
326 = P2 Outer Black (Orig White, middle button bottom row)*/

#define INPUT_CODE_BASE_VALUE (300)
#define INPUT_CODE_HIGH_VALUE (334)

#define INPUT_CODE_TO_BUTTON_INDEX(a) (a-INPUT_CODE_BASE_VALUE)

// NOTE: All of these buttons actually start at 
// 300 (304, 305, etc.)
// To save on memory/cpu, we'll remove the 300 base
// so we can create an array directly from this.
enum InputCodeToButtonMapping
{
    P1_Green     = 4,
    OptionsBack  = 5,
    P1_Yellow    = 7,
    P1_Start     = 8,
    P1_RightBot  = 10, //(Rightmost button, bottom row)
    P1_Blue      = 11,
    P1_Joy_Left  = 12,
    P1_Joy_Down  = 13,
    P1_Joy_Right = 14,
    P1_MidBot    = 15, //(Middle button, bottom row)
    P1_Joy_Up    = 16,
    P2_Start     = 20,
    P2_Joy_Right = 21,
    SelectGame   = 22,
    P2_Joy_Left  = 23,
    P2_Joy_Up    = 24,
    P2_Joy_Down  = 25,
    P2_RightBot  = 26, //(Rightmost button, bottom row)
    P2_MidBot    = 27, //(Middle button, bottom row)
    P1_Red       = 28,
    P2_Red       = 29,
    P2_Yellow    = 30,
    P2_Blue      = 32,
    P2_Green     = 33,
    
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
