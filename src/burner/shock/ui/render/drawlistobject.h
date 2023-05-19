
// See License.md for license

#ifndef DRAW_LIST_OBJECT_H_
#define DRAW_LIST_OBJECT_H_

class DrawListObject
{
public:
    friend class UIRenderer;
    
    DrawListObject( );

private:
    void Reset( );
    
    void RenderBackground( UINT16 *pBackBuffer );
    void RenderTransparentSprite( UINT16 *pBackBuffer );
    void RenderSprite( UINT16 *pBackBuffer );
    void RenderText( UINT16 *pBackBuffer );
    void RenderLine( UINT16 *pBackBuffer );
    
    void RenderToBackBuffer( UINT16 *pBackBuffer );

private:
    UINT16       *mpImageSurface;
    
    char          mText[ MAX_PATH ];
    short         mTextColor;
    FontType      mFontType;
    
    const UINT16 *mpSpriteBuffer;
    int           mSpriteWidth;
    int           mSpriteHeight;
    UINT16        mSpriteClearColor;
    int           mSpriteUseClearColor;
    
    int           mLineColor;
    int           mLineWidth;
    
    int           mPosX;
    int           mPosY;
};

#endif
