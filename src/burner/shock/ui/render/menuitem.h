
// See License.md for license

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include "shock/shock.h"

class MenuItem
{    
public:
    void Create( const char *pText, int xPos, int yPos, short textColor );
    
    void        SetText( const char *pText );
    const char *GetText( );
    
    void  SetColor( short textColor );
    short GetColor( );
    
    void Draw( );
    
    int GetXPos( ) { return mXPos; }
    int GetYPos( ) { return mYPos; }
    
private:
    int   mXPos;
    int   mYPos;
    short mTextColor;
    char  mText[ MAX_PATH ];
};

#endif
