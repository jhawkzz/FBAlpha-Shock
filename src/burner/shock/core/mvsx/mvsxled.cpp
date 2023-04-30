
// See License.md for license

#include "../../includes.h"

int MVSXLed::mP1Led;
int MVSXLed::mP2Led;
int MVSXLed::mLedHandle;
int MVSXLed::mNeedsUpdate;
int MVSXLed::mThreadRunning;
pthread_mutex_t MVSXLed::mMutexLock;
int             MVSXLed::mMutexCreated;

int MVSXLed::Create( )
{
    // init to values that are guaranteed not to be set already.
    mP1Led = 0xDEADBEEF;
    mP2Led = 0xDEADBEEF;
    
    mLedHandle = open( LED_PATH, O_WRONLY );
    if( mLedHandle < 0 )
    {
        flushPrintf( "MVSXLed::Create() Failed to open LED device\r\n" );
        
        Destroy( );
        return -1;
    }
    
    pthread_mutex_init( &mMutexLock, NULL );
    mMutexCreated = 1;
    
    pthread_t t;
    int result = pthread_create( &t, NULL, UpdateLEDVals_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "MVSXLed::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( t );
    
    return 0;
}

void MVSXLed::Destroy( )
{
    if ( mLedHandle > -1 )
    {
        close( mLedHandle );
        mLedHandle = -1;
    }
    
    if( mMutexCreated == 1 )
    {
        pthread_mutex_lock( &mMutexLock );
        pthread_mutex_unlock( &mMutexLock );
        pthread_mutex_destroy( &mMutexLock );
        
        mMutexCreated = 0;
    }
    
    mThreadRunning = 0;
}

void MVSXLed::SetLED( int playerIndex, int val )
{
    int needsUpdate = 0;
    int p1ValToSet = mP1Led;
    int p2ValToSet = mP2Led;
    
    if( playerIndex == 0 && mP1Led != val )
    {
        needsUpdate = 1;
        p1ValToSet = max( 0, min( 99, val ) );
    }
    else if ( playerIndex == 1 && mP2Led != val )
    {
        needsUpdate = 1;
        p2ValToSet = max( 0, min( 99, val ) );
    }
    
    if( needsUpdate )
    {
        pthread_mutex_lock( &mMutexLock );
        
        mNeedsUpdate = 1;
        
        mP1Led = p1ValToSet;
        mP2Led = p2ValToSet;
        
        pthread_mutex_unlock( &mMutexLock );
    }
}

int MVSXLed::GetLED( int playerIndex )
{
    return playerIndex == 0 ? mP1Led : mP2Led;
}

void *MVSXLed::UpdateLEDVals_ThreadProc( void *pArg )
{
    mThreadRunning = 1;
    
    while( mThreadRunning )
    {
        pthread_mutex_lock( &mMutexLock );
        if( mNeedsUpdate )
        {
            char ledVals[ MAX_PATH ] = { 0 };
            snprintf( ledVals, sizeof( ledVals ) -1, "%d,%d", mP1Led, mP2Led );
            
            int bytesWritten = write( mLedHandle, ledVals, strlen( ledVals ) );
            
            mNeedsUpdate = 0;
        }
        pthread_mutex_unlock( &mMutexLock );
        
        usleep( 50 * MILLI_TO_MICROSECONDS );
    }
    
    return NULL;
}
