
// See License.md for license

#ifndef SHOCK_MAIN_H_
#define SHOCK_MAIN_H_

#include "shockmain_impl.h"
#include "core/thread.h"

#define HOLD_TIME_FOR_FRONTEND_MILLI (1500)

enum ShockState
{
    ShockState_Quit = -1,
    ShockState_Loading,
    ShockState_LoadError,
    ShockState_FrontEnd,
    ShockState_Emulator,
    ShockState_Count
};

enum LoadResult
{
    LoadResult_None,
    LoadResult_Pending,
    LoadResult_Success,
    LoadResult_Failed_Load,
    LoadResult_Failed_Other,
    LoadResult_Count
};

class ShockMain : public ShockMainImpl
{
public:
    static int  Create( );
    static void Destroy( );
    static int  BeginLoad( const char *pRomset );
    static void Run( const char *pRomset );

private:
    static void Update( );

    static void *LoadThread( void *pArg );
    
    static void UpdateState_Loading( );
    static void UpdateState_LoadError( );
    static void UpdateState_FrontEnd( );
    static void UpdateState_Emulator( );

private:
    static ShockThread mLoadThread;
    static ShockState mState;
    static LoadResult mLoadResult;
    static char       mRomsetName[ MAX_PATH ];
};

#endif
