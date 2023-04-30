
// See License.md for license

#ifndef STATECREDITS_H_
#define STATECREDITS_H_

#define NUM_PAGES (7)

class StateCredits : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void RenderCredits( );
    void RenderPage0( );
    void RenderPage1( );
    void RenderPage2( );
    void RenderPage3( );
    void RenderPage4( );
    void RenderPage5( );
    void RenderPage6( );

private:    
    int       mPageNum;
    int       mNumMenuItems;
    MenuItem  mMenuItemList[ MAX_MENU_ITEMS ];
    int       mMenuSelection;
};

#endif
