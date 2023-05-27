
// See License.md for license

#ifndef FONT_H_
#define FONT_H_

#define FONT_CLEAR_COLOR     (0xF81F)
#define FONT_LETTER_COUNT    (51)
#define FONT_SPACING         (2)

#define FONT_BITS_PER_PIXEL  (16)
#define FONT_BYTES_PER_PIXEL ((FONT_BITS_PER_PIXEL + 7) / 8)
#define FONT_STRIDE(a)       (4 * (((a) * FONT_BYTES_PER_PIXEL + 3) / 4))

#define MAX_CHAR_LOOKUP      (128)

#define MET_FONT_WIDTH         (195)
#define MET_FONT_HEIGHT        (68)
#define MET_FONT_LETTER_WIDTH  (15)
#define MET_FONT_LETTER_HEIGHT (17)

#define UPHEAVAL_FONT_WIDTH         (611)
#define UPHEAVAL_FONT_HEIGHT        (176)
#define UPHEAVAL_FONT_LETTER_WIDTH  (47)
#define UPHEAVAL_FONT_LETTER_HEIGHT (44)

#include "shock/systems.h"

struct Letter
{
   UINT16 xPos;
   UINT16 yPos;
};

class FontObject
{
public:
           FontObject( );
         
    void   Create( UINT16 *pFontTexture, int bmpHeight, int bmpWidth, int letterWidth, int letterHeight, int letterKerning );
    void   Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor );
    UINT32 MeasureStringWidth( const char *pText );
 
private:
    void   RenderLetter( UINT16 *pBackBuffer, UINT16 *pLetterBuffer, UINT16 textColor );
 
private:
    UINT16 *mpFontTexture;
    Letter  mCharPositionLookup[ FONT_LETTER_COUNT ];
    int     mBmpWidth;
    int     mBmpHeight;
    int     mStride;
    int     mLetterWidth;
    int     mLetterHeight;
    int     mLetterKerning;
    int     mFullLetterWidth;
};

enum FontType
{
    FontType_Met,
    FontType_Upheaval,
    FontType_Count
};

class Font
{
    friend class FontObject;
public:
    static void   Create( );
    
    static void   Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor, FontType fontType = FontType_Met );
    static UINT32 MeasureStringWidth( const char *pText, FontType fontType = FontType_Met );
    
private:
    static char       mFont[ FONT_LETTER_COUNT ];
    static char       mCharLookup[ MAX_CHAR_LOOKUP ];
    static FontObject mFontObject[ FontType_Count ];
};

#endif
