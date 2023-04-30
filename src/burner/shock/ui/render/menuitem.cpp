
// See License.md for license

#include "../../includes.h"

void MenuItem::Create( const char *pText, int xPos, int yPos, short textColor )
{
    mXPos      = xPos;
    mYPos      = yPos;
    mTextColor = textColor;
    
    strncpy( mText, pText, sizeof( mText ) - 1 );
}

void MenuItem::SetColor( short textColor )
{
    mTextColor = textColor;
}

void MenuItem::Draw( )
{
    UIRenderer::DrawText( mText, 
                          mXPos,
                          mYPos,
                          mTextColor );
}
