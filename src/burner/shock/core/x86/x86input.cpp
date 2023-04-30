
// See License.md for license

#include "../../includes.h"

ButtonState X86Input::mButtonState[ Button_Count ];
int         X86Input::mInputFileHandle;
int         X86Input::mThreadRunning;

int X86Input::Create( )
{
	mInputFileHandle = open( KB_LOCAL_DEVICE, O_RDONLY );
	if( mInputFileHandle < 0 )
	{
		flushPrintf( "Failed to open KB_LOCAL_DEVICE: %s\r\n", KB_LOCAL_DEVICE );
        
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

void X86Input::Destroy( )
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

void *X86Input::PollInput_ThreadProc(void *data)
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

void X86Input::ReadInputs( )
{
	input_event input;
	int ret = read( mInputFileHandle, &input, sizeof( input_event ) );
	if(ret < 0)
	{
		flushPrintf( "ReadInputs( ) Failed\r\n" );
        return;
	}
    
    //flushPrintf( "input.code: %d, value: %d\r\n", input.code, input.value );
    
    // guard against inputs out of range
    int buttonIndex = input.code;
    if( buttonIndex >= P1_Start && buttonIndex < Button_Count )
    {   
        // quickly lock, set the value, release
        pthread_mutex_lock( &mButtonState[ buttonIndex ].mutexLock );
        
        mButtonState[ buttonIndex ].value = input.value;
            
        pthread_mutex_unlock( &mButtonState[ buttonIndex ].mutexLock );
    }
}

int X86Input::GetValueForInput( InputCodeToButtonMapping input )
{
    // quickly lock around the value and copy it out
    int inputVal = 0;
    
    pthread_mutex_lock( &mButtonState[ input ].mutexLock );
    inputVal = mButtonState[ input ].value != 0;
    pthread_mutex_unlock( &mButtonState[ input ].mutexLock );
    
    return inputVal;
}
