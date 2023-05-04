
// See License.md for license

#ifndef SHOCKUI_H_
#define SHOCKUI_H_

class ShockUI
{
public:
    static void Create( );
    static void Destroy( );
    
    static void SetState_Load( );
    static void SetState_LoadError( );
    static void SetState_MainMenu( );
    
    static int  Update( );
    
private:
    static void ChangeState( UIState newState );
    static int  UpdateState( );

private:
    static UIState               mUIState;
    static StateMainMenu         mState_MainMenu;
    static StateButtonConfig     mState_ButtonConfig;
    static StateGameSettings     mState_GameSettings;
    static StateDisplaySettings  mState_DisplaySettings;
    static StateEmulatorSettings mState_EmulatorSettings;
    static StateCredits          mState_Credits;
    static StateLoading          mState_Loading;
    static StateLoadError        mState_LoadError;
    static UIBaseState          *mpStateList[ UIState_Count ];
    static UINT16                mBackgroundImage[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT ];
    static long                  mFrameTimeMS;
};

#endif
