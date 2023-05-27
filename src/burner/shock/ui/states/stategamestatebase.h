
// See License.md for license

#ifndef STATEGAMESTATEBASE_H_
#define STATEGAMESTATEBASE_H_

#define THUMB_IMAGE_SPACING (50)

class StateGameStateBase : public UIBaseState
{
public:
    virtual void Create();
    virtual void Destroy();

    virtual void EnterState(UIState oldState);
    virtual void ExitState(UIState newState);

    virtual UIState Update();

protected:
    virtual void DrawMenu();

    int          mNumMenuItems;
    MenuItem     mMenuItemList[MAX_MENU_ITEMS];
    UINT16       mStateThumb[MAX_SAVE_STATES][STATE_THUMBNAIL_WIDTH * STATE_THUMBNAIL_HEIGHT];
    int          mStateExists[MAX_SAVE_STATES];
    UINT16       mStateBlankImg[STATE_THUMBNAIL_WIDTH * STATE_THUMBNAIL_HEIGHT];
                 
    int          mMenuSelection;
    char         mResultStr[MAX_PATH];
    int          mMenuItemFullWidth;
};

#endif
