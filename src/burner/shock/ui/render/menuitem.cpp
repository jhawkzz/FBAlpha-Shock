
// See License.md for license

#include "shock/ui/render/menuitem.h"
#include "shock/ui/render/uirenderer.h"

void MenuItem::Create( const char *pText, int xPos, int yPos, short textColor )
{
    mXPos      = xPos;
    mYPos      = yPos;
    mTextColor = textColor;
    
    strncpy( mText, pText, sizeof( mText ) - 1 );
}

void MenuItem::SetText( const char *pText )
{
    strncpy( mText, pText, sizeof( mText ) - 1 );
}

const char *MenuItem::GetText( )
{
    return mText;
}

void MenuItem::SetColor( short textColor )
{
    mTextColor = textColor;
}

short MenuItem::GetColor( )
{
    return mTextColor;
}

void MenuItem::Draw( )
{
    UIRenderer::DrawText( mText, 
                          mXPos,
                          mYPos,
                          mTextColor );
}
