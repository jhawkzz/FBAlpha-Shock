
// See License.md for license

#ifndef SHOCK_MAIN_H_
#define SHOCK_MAIN_H_

#define HOLD_TIME_FOR_FRONTEND_MILLI (1500)

enum ShockState
{
    ShockState_Idle,
    ShockState_Loading,
    ShockState_FrontEnd,
    ShockState_Emulator,
    ShockState_Count
};

enum LoadResult
{
    LoadResult_None,
    LoadResult_Pending,
    LoadResult_Success,
    LoadResult_Failed,
    LoadResult_Count
};

class ShockMain
{
public:
    static int  Create( );
    static void Destroy( );
    static int  BeginLoad( char *pRomset );
    static int  Update( );

private:
    static void *LoadThread( void *pArg );
    
    static int   UpdateState_Loading( );
    static int   UpdateState_FrontEnd( );
    static int   UpdateState_Emulator( );

private:
    static ShockState mState;
    static LoadResult mLoadResult;
    static char       mRomsetName[ MAX_PATH ];
};

#endif
