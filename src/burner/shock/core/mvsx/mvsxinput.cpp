
// See License.md for license

#include "../../includes.h"

ButtonState MVSXInput::mButtonState[ Button_Count ];
int         MVSXInput::mInputFileHandle;
int         MVSXInput::mThreadRunning;

int MVSXInput::Create( )
{
	mInputFileHandle = open( SNK_JS_LOCAL_DEVICE, O_RDONLY );
	if( mInputFileHandle < 0 )
	{
		flushPrintf( "Failed to open SNK_JS_LOCAL_DEVICE\r\n" );
        
        Destroy( );
        return -1;
	}
    
    // setup all our inputs
    for( int i = 0; i < Button_Count; i++ )
    {
        mButtonState[ i ].value = 0;
        pthread_mutex_init( &mButtonState[ i ].mutexLock, NULL );
        mButtonState[ i ].mutexCreated = 1;
    }
	
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

void MVSXInput::Destroy( )
{
    for( int i = 0; i < Button_Count; i++ )
    {
        if ( mButtonState[ i ].mutexCreated == 1 )
        {
            pthread_mutex_lock( &mButtonState[ i ].mutexLock );
            pthread_mutex_unlock( &mButtonState[ i ].mutexLock );
            pthread_mutex_destroy( &mButtonState[ i ].mutexLock );
            
            mButtonState[ i ].mutexCreated = 0;
        }
    }
    
    if( mInputFileHandle > -1 )
    {
        close( mInputFileHandle );
        mInputFileHandle = -1;
    }
    
    mThreadRunning = 0;
}

void *MVSXInput::PollInput_ThreadProc(void *data)
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

void MVSXInput::ReadInputs( )
{
	input_event input;
	int ret = read( mInputFileHandle, &input, sizeof( input_event ) );
	if(ret < 0)
	{
		flushPrintf( "ReadInputs( ) Failed\r\n" );
        return;
	}
    
    // guard against inputs out of range
    if( input.code >= INPUT_CODE_BASE_VALUE && input.code < INPUT_CODE_HIGH_VALUE )
    {
        int buttonIndex = INPUT_CODE_TO_BUTTON_INDEX(input.code);
        
        // quickly lock, set the value, release
        pthread_mutex_lock( &mButtonState[ buttonIndex ].mutexLock );
        
        mButtonState[ buttonIndex ].value = input.value;
        
        pthread_mutex_unlock( &mButtonState[ buttonIndex ].mutexLock );
    }
}

int MVSXInput::GetValueForInput( InputCodeToButtonMapping input )
{
    // quickly lock around the value and copy it out
    int inputVal = 0;
    
    pthread_mutex_lock( &mButtonState[ input ].mutexLock );
    inputVal = mButtonState[ input ].value;
    pthread_mutex_unlock( &mButtonState[ input ].mutexLock );
    
    return inputVal;
}
