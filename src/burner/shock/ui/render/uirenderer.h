
// See License.md for license

#ifndef UIRENDERER_H_
#define UIRENDERER_H_

#define MAX_DRAWLIST_OBJECTS (256)

class DrawListObject;

class UIRenderer
{
public:
    static void Create( );
    static void Destroy( );
    
    static void DrawBackground( UINT16 *pImageSurface );
    static void DrawTransparentSprite( UINT16 *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight, UINT16 spriteClearColor );
    static void DrawSprite( UINT16 *pSpriteBuffer, int x, int y, int spriteWidth, int spriteHeight );
    static void DrawText( const char *pText, int x, int y, UINT16 color, FontType fontType = FontType_Met );
    
    static void ResetForFrame( );
    static void Render( );
    static void Flip( );
    
private:
    static int 		      mDrawListCount;
    static DrawListObject mDrawList[ MAX_DRAWLIST_OBJECTS ];
};

#endif
