
// See License.md for license

#ifndef STATESAVESTATE_H_
#define STATESAVESTATE_H_

#include "shock/ui/states/stategamestatebase.h"

class StateSaveState : public StateGameStateBase
{
public:
    virtual void Create( );
    virtual void Destroy( );

    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );

    virtual UIState Update( );

protected:
    // used to let ShockGame write into while on another thread
    UINT16 mStateThumbBuffer[ STATE_THUMBNAIL_WIDTH * STATE_THUMBNAIL_HEIGHT ];
};

#endif
