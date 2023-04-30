
// See License.md for license

#ifndef MENUITEM_H_
#define MENUITEM_H_

class MenuItem
{    
public:
    void Create( const char *pText, int xPos, int yPos, short textColor );
    
    void SetColor( short textColor );
    
    void Draw( );
    
    int  GetXPos( ) { return mXPos; }
    int  GetYPos( ) { return mYPos; }
    
private:
    int   mXPos;
    int   mYPos;
    short mTextColor;
    char  mText[ MAX_PATH ];
};

#endif
