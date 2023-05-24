
// See License.md for license

#include "shock/core/input.h"
#include "shock/util/util.h"

LubuntuInput      InputCore::mLubuntuInputLookup[ ShockButton_Count ];
LubuntuInputState InputCore::mLubuntuInputState[ LubuntuInput_Count ];
int               InputCore::mInputFileHandle;
int               InputCore::mThreadRunning;

int Input::Create( )
{
	mInputFileHandle = open( KB_LOCAL_DEVICE, O_RDONLY );
	if( mInputFileHandle < 0 )
	{
		flushPrintf( "Failed to open KB_LOCAL_DEVICE: %s\r\n", KB_LOCAL_DEVICE );
        
        Destroy( );
        return -1;
	}
    
    // setup all our inputs
    for( int i = 0; i < LubuntuInput_Count; i++ )
    {
        mLubuntuInputState[ i ].value = 0;
        pthread_mutex_init( &mLubuntuInputState[ i ].mutexLock, NULL );
        mLubuntuInputState[ i ].mutexCreated = 1;
    }
    
    CreateLookup( );
	
    // start the polling thread
	pthread_t t;
	int result = pthread_create( &t, NULL, PollInput_ThreadProc, NULL );
	if( result != 0 )
	{
        flushPrintf( "Failed to start Poll Thread\r\n" );
        
        Destroy( );
        return -1;
	}
    
    pthread_detach( t );
    
    return 0;
}

void Input::Destroy( )
{
    for( int i = 0; i < ShockButton_Count; i++ )
    {
        if ( mLubuntuInputState[ i ].mutexCreated == 1 )
        {
            pthread_mutex_lock( &mLubuntuInputState[ i ].mutexLock );
            pthread_mutex_unlock( &mLubuntuInputState[ i ].mutexLock );
            pthread_mutex_destroy( &mLubuntuInputState[ i ].mutexLock );
            
            mLubuntuInputState[ i ].mutexCreated = 0;
        }
    }
    
    if( mInputFileHandle > -1 )
    {
        close( mInputFileHandle );
        mInputFileHandle = -1;
    }
    
    mThreadRunning = 0;
}

void Input::CreateLookup( )
{
    mLubuntuInputLookup[ P1_Joy_Up     ] = LubuntuInput_KB_Up;
    mLubuntuInputLookup[ P1_Joy_Left   ] = LubuntuInput_KB_Left,
    mLubuntuInputLookup[ P1_Joy_Right  ] = LubuntuInput_KB_Right,
    mLubuntuInputLookup[ P1_Joy_Down   ] = LubuntuInput_KB_Down,
    
    mLubuntuInputLookup[ P1_Button_1   ] = LubuntuInput_KB_Z;
    mLubuntuInputLookup[ P1_Button_2   ] = LubuntuInput_KB_A;
    mLubuntuInputLookup[ P1_Button_3   ] = LubuntuInput_KB_S;
    mLubuntuInputLookup[ P1_Button_4   ] = LubuntuInput_KB_D;
    mLubuntuInputLookup[ P1_Button_5   ] = LubuntuInput_KB_X;
    mLubuntuInputLookup[ P1_Button_6   ] = LubuntuInput_KB_C;
    
    mLubuntuInputLookup[ P1_InsertCoin ] = LubuntuInput_KB_5;
    mLubuntuInputLookup[ P1_Start      ] = LubuntuInput_KB_1;
    
    // Player 2
    mLubuntuInputLookup[ P2_Joy_Up     ] = LubuntuInput_KB_Home;
    mLubuntuInputLookup[ P2_Joy_Left   ] = LubuntuInput_KB_Delete;
    mLubuntuInputLookup[ P2_Joy_Right  ] = LubuntuInput_KB_PgDn;
    mLubuntuInputLookup[ P2_Joy_Down   ] = LubuntuInput_KB_End;
    
    mLubuntuInputLookup[ P2_Button_1   ] = LubuntuInput_KB_H;
    mLubuntuInputLookup[ P2_Button_2   ] = LubuntuInput_KB_Y;
    mLubuntuInputLookup[ P2_Button_3   ] = LubuntuInput_KB_U;
    mLubuntuInputLookup[ P2_Button_4   ] = LubuntuInput_KB_I;
    mLubuntuInputLookup[ P2_Button_5   ] = LubuntuInput_KB_J;
    mLubuntuInputLookup[ P2_Button_6   ] = LubuntuInput_KB_K;
    
    mLubuntuInputLookup[ P2_InsertCoin ] = LubuntuInput_KB_6;
    mLubuntuInputLookup[ P2_Start      ] = LubuntuInput_KB_2;
}

void *InputCore::PollInput_ThreadProc(void *data)
{
    mThreadRunning = 1;
	
    while( mThreadRunning )
	{
        pollfd inputState;
		memset( &inputState, 0, sizeof( pollfd ) );
        
		inputState.fd = mInputFileHandle;
		inputState.events = POLLIN;
		inputState.revents = 0;
		int ret = poll( &inputState, 1, INPUT_POLL_TIMEOUT );
		if( ret >= 0 )
		{
			ReadInputs( );
		}
        else
        {
            flushPrintf( "Failed to poll input. Skipping read inputs, will try again next iteration.\r\n" );
        }
	}
    
    return NULL;
}

void InputCore::ReadInputs( )
{
	input_event input;
	int ret = read( mInputFileHandle, &input, sizeof( input_event ) );
	if(ret < 0)
	{
		flushPrintf( "ReadInputs( ) Failed\r\n" );
        return;
	}
    
    // guard against inputs out of range
    if( input.code >= LubuntuInput_KB_1 && input.code < LubuntuInput_Count )
    {   
        // quickly lock, set the value, release
        pthread_mutex_lock( &mLubuntuInputState[ input.code ].mutexLock );
        
        mLubuntuInputState[ input.code ].value = input.value;
            
        pthread_mutex_unlock( &mLubuntuInputState[ input.code ].mutexLock );
    }
}

int Input::GetValueForButton( ShockButton shockButton )
{
    // quickly lock around the value and copy it out
    int inputVal = 0;
    
    LubuntuInput input = mLubuntuInputLookup[ shockButton ];
    
    pthread_mutex_lock( &mLubuntuInputState[ input ].mutexLock );
    inputVal = mLubuntuInputState[ input ].value != 0;
    pthread_mutex_unlock( &mLubuntuInputState[ input ].mutexLock );
    
    return inputVal;
}
