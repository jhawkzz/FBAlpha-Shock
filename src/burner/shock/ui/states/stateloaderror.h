
// See License.md for license

#ifndef STATELOADERROR_H_
#define STATELOADERROR_H_

#include "shock/ui/render/menuitem.h"
#include "shock/ui/states/uibasestate.h"

class StateLoadError : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );    
    
            int     ShouldExitEmulator( );
            int     ShouldExitUI( );
    
private:
    void RenderFatalError( );
    void RenderWarning( );
    
private:
    int mRomLoadResult;
    int mExitEmulator;
    int mExitUI;
};

#endif
