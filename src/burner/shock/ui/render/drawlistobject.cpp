
// See License.md for license

#include "shock/ui/render/drawlistobject.h"

DrawListObject::DrawListObject( )
{
    memset( mText, 0, sizeof( mText ) );
    
    mTextColor = 0;
    mFontType  = (FontType)0;
    
    mLineWidth = 0;
    mLineColor = 0;
    
    mSpriteWidth         = 0;
    mSpriteHeight        = 0;
    mSpriteClearColor    = 0;
    mSpriteUseClearColor = 0;
    mpSpriteBuffer       = NULL;
    
    mPosX = 0;
    mPosY = 0;
    
    mpImageSurface = NULL;
}

void DrawListObject::Reset( )
{
    mpImageSurface = NULL;
    mText[ 0 ]     = 0;
    mpSpriteBuffer = NULL;
    
    mTextColor = 0;
    mFontType  = (FontType)0;
    
    mSpriteWidth         = 0;
    mSpriteHeight        = 0;
    mSpriteClearColor    = 0;
    mSpriteUseClearColor = 0;
    
    mPosX = 0;
    mPosY = 0;
}

void DrawListObject::RenderBackground( UINT16 *pBackBuffer )
{
    memcpy( (char *)pBackBuffer, (char *)mpImageSurface, UI_HEIGHT * UI_WIDTH * FRAMEBUFFER_BYTES_PP );
}

void DrawListObject::RenderTransparentSprite( UINT16 *pBackBuffer )
{
    UINT16 *pShortBackBuffer = pBackBuffer + (mPosX + (mPosY * UI_WIDTH));
    
    // clip dimensions to the edge of the screen
    int spriteDrawWidth = min( mSpriteWidth, UI_WIDTH - mPosX );
    int spriteDrawHeight = min( mSpriteHeight, UI_HEIGHT - mPosY );
    
    for( int y = 0; y < spriteDrawHeight; y++ )
    {
        for( int x = 0; x < spriteDrawWidth; x++ )
        {
            if( mpSpriteBuffer[ x ] != mSpriteClearColor )
            {
                pShortBackBuffer[ x ] = mpSpriteBuffer[ x ];
            }
        }
        
        pShortBackBuffer += UI_WIDTH;
        mpSpriteBuffer += mSpriteWidth;
    }
}

void DrawListObject::RenderSprite( UINT16 *pBackBuffer )
{
    UINT16 *pShortBackBuffer = pBackBuffer + (mPosX + (mPosY * UI_WIDTH));
    
    // clip dimensions to the edge of the screen
    int spriteDrawWidth = min( mSpriteWidth, UI_WIDTH - mPosX );
    int spriteDrawHeight = min( mSpriteHeight, UI_HEIGHT - mPosY );
    
    for( int y = 0; y < spriteDrawHeight; y++ )
    {
        for( int x = 0; x < spriteDrawWidth; x++ )
        {
            pShortBackBuffer[ x ] = mpSpriteBuffer[ x ];
        }
        
        pShortBackBuffer += UI_WIDTH;
        mpSpriteBuffer += mSpriteWidth;
    }
}

void DrawListObject::RenderText( UINT16 *pBackBuffer )
{
    Font::Print( mText, mPosX, mPosY, mTextColor, mFontType );
}

void DrawListObject::RenderLine( UINT16 *pBackBuffer )
{
    UINT16 *pShortBackBuffer = pBackBuffer + (mPosX + (mPosY * UI_WIDTH));
    
    int availWidth = UI_WIDTH - mPosX;
    int length = min( mLineWidth, availWidth );
    
    for( int x = 0; x < length; x++ )
    {
        pShortBackBuffer[ x ] = mLineColor;
    }
}

void DrawListObject::RenderToBackBuffer( UINT16 *pBackBuffer )
{
    if( mpImageSurface != NULL )
    {
        RenderBackground( pBackBuffer );
    }
    else if ( mText[ 0 ] != 0 )
    {
        RenderText( pBackBuffer );
    }
    else if( mpSpriteBuffer != NULL )
    {
        if( mSpriteUseClearColor )
        {
            RenderTransparentSprite( pBackBuffer );
        }
        else
        {
            RenderSprite( pBackBuffer );
        }
    }
    else if ( mLineWidth > 0 )
    {
        RenderLine( pBackBuffer );
    }
}
