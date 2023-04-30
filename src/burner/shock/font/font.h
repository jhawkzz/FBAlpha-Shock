
// See License.md for license

#ifndef FONT_H_
#define FONT_H_

#define FONT_CLEAR_COLOR       (0xF81F)
#define FONT_WIDTH             (195)
#define FONT_HEIGHT            (68)
#define FONT_BITS_PER_PIXEL    (16)
#define FONT_BYTES_PER_PIXEL   ((FONT_BITS_PER_PIXEL + 7) / 8)
#define FONT_STRIDE            (4 * ((FONT_WIDTH * FONT_BYTES_PER_PIXEL + 3) / 4))
#define FONT_LETTER_COUNT      (44)
#define FONT_LETTER_WIDTH      (15)
#define FONT_LETTER_HEIGHT     (17)
#define FONT_SPACING           (2)
#define FONT_LETTER_FULL_WIDTH (FONT_LETTER_WIDTH + FONT_SPACING)

struct Letter
{
   UINT8 xPos;
   UINT8 yPos;
};

class Font
{
public:
    static void   Create( );
    static void   Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor );
    static UINT32 MeasureStringWidth( const char *pText );

private:
    static void   RenderLetter( UINT16 *pBackBuffer, UINT16 *pLetterBuffer, UINT16 textColor );
    
    static UINT16 *mpFontTexture;
    static char    mCharLookup[ 128 ];
    static char    mFont[ FONT_LETTER_COUNT ];
    static Letter  mCharPositionLookup[ FONT_LETTER_COUNT ];
};

#endif
