
// See License.md for license

#include "../../includes.h"

int            UIRenderer::mDrawListCount;
DrawListObject UIRenderer::mDrawList[ MAX_DRAWLIST_OBJECTS ];

void UIRenderer::Create( )
{
    mDrawListCount = 0;
}

void UIRenderer::Destroy( )
{
    mDrawListCount = 0;
}

void UIRenderer::DrawBackground( UINT16 *pImageSurface )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mpImageSurface  = pImageSurface;
        
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add image.\r\n" );
    }
}

void UIRenderer::DrawTransparentSprite( UINT16 *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight, UINT16 spriteClearColor )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mpSpriteBuffer  = pSpriteBuffer;
        
        mDrawList[ mDrawListCount ].mPosX = x;
        mDrawList[ mDrawListCount ].mPosY = y;
        
        mDrawList[ mDrawListCount ].mSpriteWidth = spriteWidth;
        mDrawList[ mDrawListCount ].mSpriteHeight = spriteHeight;
        mDrawList[ mDrawListCount ].mSpriteClearColor = spriteClearColor;
        mDrawList[ mDrawListCount ].mSpriteUseClearColor = 1;
        
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add sprite.\r\n" );
    }
}

void UIRenderer::DrawSprite( UINT16 *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mpSpriteBuffer  = pSpriteBuffer;
        
        mDrawList[ mDrawListCount ].mPosX = x;
        mDrawList[ mDrawListCount ].mPosY = y;
        
        mDrawList[ mDrawListCount ].mSpriteWidth = spriteWidth;
        mDrawList[ mDrawListCount ].mSpriteHeight = spriteHeight;
        
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add sprite.\r\n" );
    }
}

void UIRenderer::DrawText( const char *pText, int x, int y, UINT16 color, FontType fontType/*= FontType_Met*/ )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        strncpy( mDrawList[ mDrawListCount ].mText, pText, sizeof( mDrawList[ mDrawListCount ].mText ) - 1 );
        
        mDrawList[ mDrawListCount ].mPosX = x;
        mDrawList[ mDrawListCount ].mPosY = y;
        
        mDrawList[ mDrawListCount ].mTextColor = color;
        mDrawList[ mDrawListCount ].mFontType = fontType;
                
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add text.\r\n" );
    }
}

void UIRenderer::DrawLine( UINT16 color, int xPos, int yPos, int width )
{
    if( mDrawListCount + 1 < MAX_DRAWLIST_OBJECTS )
    {
        mDrawList[ mDrawListCount ].Reset( );
        
        mDrawList[ mDrawListCount ].mLineColor = color;
        mDrawList[ mDrawListCount ].mLineWidth = width;
        mDrawList[ mDrawListCount ].mPosX      = xPos;
        mDrawList[ mDrawListCount ].mPosY      = yPos;
                
        mDrawListCount++;
    }
    else
    {
        flushPrintf( "Draw List Full. Cannot add text.\r\n" );
    }
}

void UIRenderer::ResetForFrame( )
{
    mDrawListCount = 0;
}

void UIRenderer::Render( )
{
    UINT16 *pBackBuffer = (UINT16 *)ShockRenderer::GetBackBuffer( );
       
    if( pBackBuffer != NULL )
    {
        for( int i = 0; i < mDrawListCount; i++ )
        {
            mDrawList[ i ].RenderToBackBuffer( pBackBuffer );
        }
    }
}

void UIRenderer::Flip( )
{
    ShockRenderer::Flip( );
}
