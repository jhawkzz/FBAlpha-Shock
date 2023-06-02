
// See License.md for license

#ifndef STATELOADSTATE_H_
#define STATELOADSTATE_H_

#include "shock/ui/states/stategamestatebase.h"

class StateLoadState : public StateGameStateBase
{
public:
    virtual void Create( );
    virtual void Destroy( );

    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );

    virtual UIState Update( );

protected:
    virtual void DrawMenu( );
};

#endif
