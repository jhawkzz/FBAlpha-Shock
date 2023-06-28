
// See License.md for license

#ifndef STATELOADING_H_
#define STATELOADING_H_

#include "shock/ui/render/menuitem.h"
#include "shock/ui/states/uibasestate.h"

class StateLoading : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void      DrawMenu( );
    
private:
    int    mLoadingColorLetterIndex;
    UINT64 mLoadingTimerMS;
};

#endif
