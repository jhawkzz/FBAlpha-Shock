
// See License.md for license

#include "shock/core/button.h"
#include "shock/core/input.h"
#include "shock/util/util.h"

MVSXInput      InputCore::mMVSXInputLookup[ ShockButton_Count ];
MVSXInputState InputCore::mMVSXInputState[ MVSXInput_Count ];
int            InputCore::mInputFileHandle;
int            InputCore::mThreadRunning;

int Input::Create( )
{
	mInputFileHandle = open( MVSX_INTEGRATED_DEVICE, O_RDONLY );
	if( mInputFileHandle < 0 )
	{
		flushPrintf( "Failed to open MVSX_INTEGRATED_DEVICE\r\n" );
        
        Destroy( );
        return -1;
	}
    
    // setup all our inputs
    for( int i = 0; i < MVSXInput_Count; i++ )
    {
        mMVSXInputState[ i ].value = 0;
        pthread_mutex_init( &mMVSXInputState[ i ].mutexLock, NULL );
        mMVSXInputState[ i ].mutexCreated = 1;
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
    for( int i = 0; i < MVSXInput_Count; i++ )
    {
        if ( mMVSXInputState[ i ].mutexCreated == 1 )
        {
            pthread_mutex_lock( &mMVSXInputState[ i ].mutexLock );
            pthread_mutex_unlock( &mMVSXInputState[ i ].mutexLock );
            pthread_mutex_destroy( &mMVSXInputState[ i ].mutexLock );
            
            mMVSXInputState[ i ].mutexCreated = 0;
        }
    }
    
    if( mInputFileHandle > -1 )
    {
        close( mInputFileHandle );
        mInputFileHandle = -1;
    }
    
    mThreadRunning = 0;
}

void InputCore::CreateLookup( )
{
    mMVSXInputLookup[ P1_Joy_Up     ] = MVSXInput_P1_Joy_Up;
    mMVSXInputLookup[ P1_Joy_Left   ] = MVSXInput_P1_Joy_Left;
    mMVSXInputLookup[ P1_Joy_Right  ] = MVSXInput_P1_Joy_Right;
    mMVSXInputLookup[ P1_Joy_Down   ] = MVSXInput_P1_Joy_Down;
    
    mMVSXInputLookup[ P1_Button_1   ] = MVSXInput_P1_Red;
    mMVSXInputLookup[ P1_Button_2   ] = MVSXInput_P1_Yellow;
    mMVSXInputLookup[ P1_Button_3   ] = MVSXInput_P1_Green;
    mMVSXInputLookup[ P1_Button_4   ] = MVSXInput_P1_Blue;
    mMVSXInputLookup[ P1_Button_5   ] = MVSXInput_P1_MidBot;
    mMVSXInputLookup[ P1_Button_6   ] = MVSXInput_P1_RightBot;
    
    mMVSXInputLookup[ P1_InsertCoin ] = MVSXInput_OptionsBack;
    mMVSXInputLookup[ P1_Start      ] = MVSXInput_P1_Start;
    
    // Player 2
    mMVSXInputLookup[ P2_Joy_Up     ] = MVSXInput_P2_Joy_Up;
    mMVSXInputLookup[ P2_Joy_Left   ] = MVSXInput_P2_Joy_Left;
    mMVSXInputLookup[ P2_Joy_Right  ] = MVSXInput_P2_Joy_Right;
    mMVSXInputLookup[ P2_Joy_Down   ] = MVSXInput_P2_Joy_Down;
    
    mMVSXInputLookup[ P2_Button_1   ] = MVSXInput_P2_Red;
    mMVSXInputLookup[ P2_Button_2   ] = MVSXInput_P2_Yellow;
    mMVSXInputLookup[ P2_Button_3   ] = MVSXInput_P2_Green;
    mMVSXInputLookup[ P2_Button_4   ] = MVSXInput_P2_Blue;
    mMVSXInputLookup[ P2_Button_5   ] = MVSXInput_P2_MidBot;
    mMVSXInputLookup[ P2_Button_6   ] = MVSXInput_P2_RightBot;
    
    mMVSXInputLookup[ P2_InsertCoin ] = MVSXInput_SelectGame;
    mMVSXInputLookup[ P2_Start      ] = MVSXInput_P2_Start;
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
    if( input.code >= MVSXInput_P1_Green && input.code < MVSXInput_Count )
    {
        // quickly lock, set the value, release
        pthread_mutex_lock( &mMVSXInputState[ input.code ].mutexLock );
        
        mMVSXInputState[ input.code ].value = input.value;
        
        pthread_mutex_unlock( &mMVSXInputState[ input.code ].mutexLock );
    }
}

int Input::GetValueForButton( ShockButton shockButton )
{
    // quickly lock around the value and copy it out
    int inputVal = 0;
    
    MVSXInput input = mMVSXInputLookup[ shockButton ];
    
    pthread_mutex_lock( &mMVSXInputState[ input ].mutexLock );
    inputVal = mMVSXInputState[ input ].value;
    pthread_mutex_unlock( &mMVSXInputState[ input ].mutexLock );
    
    return inputVal;
}
