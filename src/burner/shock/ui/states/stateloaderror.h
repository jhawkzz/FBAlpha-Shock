
// See License.md for license

#ifndef STATELOADERROR_H_
#define STATELOADERROR_H_

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
    int mDisplayFatalError;
    
    int mExitEmulator;
    int mExitUI;
};

#endif
