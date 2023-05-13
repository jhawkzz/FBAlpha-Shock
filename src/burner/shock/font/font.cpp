
// See License.md for license

#include "../includes.h"
#include "shock/font/metfontbinary.h"
#include "shock/font/upheavalfontbinary.h"

//todo: bug when rendering the &, its rendering the top line of '9' as the last line of &
// proably wrapping the buffer somehow. i notice it whenever rendering any certain letters on the bottom row.

char Font::mFont[ FONT_LETTER_COUNT ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
                                          'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
                                          'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ',', '.', '!',
                                          '%', ':', '#', '-', '/', '\\', '(', ')', '*', '&', '@' }; //# = Copyright logo

char       Font::mCharLookup[ MAX_CHAR_LOOKUP ];
FontObject Font::mFontObject[ FontType_Count ];

void Font::Create( )
{
    // create our lookup table
    int i;
    for ( i = 0; i < FONT_LETTER_COUNT; i++ )
    {
        if( mFont[ i ] >= MAX_CHAR_LOOKUP )
        {
            flushPrintf( "ERROR: Font::CreateLookup() mFont[ i ] out of range with val: %d\r\n", mFont[ i ] );
            exit( 0 );
        }
        
        mCharLookup[ mFont[ i ] ] = (char) i;

        // minus 13 (this is the last alphanumeric letter, 'Z')
        if ( i < FONT_LETTER_COUNT - 14 )
        {
            if( mFont[ i ] + 32 >= MAX_CHAR_LOOKUP )
            {
                flushPrintf( "ERROR: Font::CreateLookup() mFont[ i ] + 32 out of range with val: %d\r\n", mFont[ i ] + 32 );
                exit( 0 );
            }
            
            // force lowercase to use the same indices
            mCharLookup[ mFont[ i ] + 32 ] = (char) i; 
        }
    }
    
    // create our fonts
    mFontObject[ FontType_Met ].Create( (UINT16 *)gMetFontBytes, 
                                        MET_FONT_WIDTH, 
                                        MET_FONT_HEIGHT, 
                                        MET_FONT_LETTER_WIDTH, 
                                        MET_FONT_LETTER_HEIGHT, 
                                        FONT_SPACING );
                                        
    mFontObject[ FontType_Upheaval ].Create( (UINT16 *)gUpheavalFontBytes, 
                                             UPHEAVAL_FONT_WIDTH, 
                                             UPHEAVAL_FONT_HEIGHT, 
                                             UPHEAVAL_FONT_LETTER_WIDTH, 
                                             UPHEAVAL_FONT_LETTER_HEIGHT, 
                                             FONT_SPACING );
}

void Font::Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor, FontType fontType/* = FontType_Met*/ )
{
    /*InternalPrint( pBackBuffer, "0123456", 0, 100, 0xFFFF );
    InternalPrint( pBackBuffer, "789ABCD",  0, 200, 0xFFFF );
    InternalPrint( pBackBuffer, "EFGHIJK",  0, 300, 0xFFFF );
    
    InternalPrint( pBackBuffer, "LMNOPQ",  0, 100, 0xFFFF );
    InternalPrint( pBackBuffer, "RSTUVW",  0, 200, 0xFFFF );
    InternalPrint( pBackBuffer, "XYZ,.!%",  0, 300, 0xFFFF );
    InternalPrint( pBackBuffer, "#-/\\()*",  0, 400, 0xFFFF );
    InternalPrint( pBackBuffer, "&",  0, 500, 0xFFFF );*/
    if( fontType >= 0 && fontType < FontType_Count )
    {
        mFontObject[ fontType ].Print( pBackBuffer, pString, xPos, yPos, textColor );
    }
}

UINT32 Font::MeasureStringWidth( const char *pText, FontType fontType/* = FontType_Met*/ )
{
    if( fontType >= 0 && fontType < FontType_Count )
    {
        return mFontObject[ fontType ].MeasureStringWidth( pText );
    }
    
    return 0;
}

FontObject::FontObject( )
{
    mpFontTexture = NULL;
    memset( mCharPositionLookup, 0, sizeof( mCharPositionLookup ) );
    
    mBmpWidth        = 0;
    mBmpHeight       = 0;
    mStride          = 0;
    mLetterWidth     = 0;
    mLetterHeight    = 0;
    mLetterKerning   = 0;
    mFullLetterWidth = 0;
}

void FontObject::Create( UINT16 *pFontTexture, 
                         int bmpWidth, 
                         int bmpHeight, 
                         int letterWidth, 
                         int letterHeight, 
                         int letterKerning )
{
    mpFontTexture    = pFontTexture;
    mBmpWidth        = bmpWidth;
    mBmpHeight       = bmpHeight;
    mStride          = FONT_STRIDE(bmpWidth);
    mLetterWidth     = letterWidth;
    mLetterHeight    = letterHeight;
    mLetterKerning   = letterKerning; //this should eventually be per-letter
    mFullLetterWidth = letterWidth + mLetterKerning;
    
    // setup our lookup tables
    int lettersPerRow = bmpWidth / letterWidth;
    
    int i;
    for ( i = 0; i < FONT_LETTER_COUNT; i++ )
    {
        // fill our char position lookup (where the X/Y pixel start for each letter is)
        mCharPositionLookup[ i ].xPos = (i % lettersPerRow) * letterWidth;
        mCharPositionLookup[ i ].yPos = (i / lettersPerRow) * letterHeight;
    }
}

void FontObject::Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor )
{
    // clamp height
    yPos = min( yPos, PLATFORM_LCD_HEIGHT - mLetterHeight );
    
    // clamp width (by limiting string length)
    int allowedStringLen = strlen( pString );
    
    int maxWidth = PLATFORM_LCD_WIDTH - xPos;
    int stringWidth = (allowedStringLen * mFullLetterWidth) - FONT_SPACING;
    
    // is to too long?
    int delta = stringWidth - maxWidth;
    if( delta > 0 )
    {
        // calculate how many letters are safe to render
        int allowedWidth = stringWidth - delta;
        allowedStringLen = allowedWidth / mFullLetterWidth;
    }
    
    // position the screen buffer where the text should start
    UINT16 *pCurrBuffer = pBackBuffer + ( yPos * PLATFORM_LCD_WIDTH ) + xPos;

    int strIndex = 0;
    while( strIndex < allowedStringLen )
    {
        switch( pString[ strIndex ] )
        {
            default:
            {     
                // get the index of the letter in our font
                int letter_index = Font::mCharLookup[ pString[ strIndex ] ];
            
                // render the letter to the screen
                UINT16 *pLetter = mpFontTexture 
                                + ( mCharPositionLookup[ letter_index ].yPos * (mStride / 2) ) 
                                +   mCharPositionLookup[ letter_index ].xPos;
                                
                RenderLetter( pCurrBuffer, pLetter, textColor );

                break;
            }
            
            // Space
            case 32:
            {
                break;
            }
        }

        pCurrBuffer += mFullLetterWidth;

        strIndex++;
    }
}

UINT32 FontObject::MeasureStringWidth( const char *pText )
{
    return strlen( pText ) * mFullLetterWidth;
}

void FontObject::RenderLetter( UINT16 *pBackBuffer, UINT16 *pLetterBuffer, UINT16 textColor )
{
    int y;
    for ( y = 0; y < mLetterHeight; y++ )
    {
        int x;
        for ( x = 0; x < mLetterWidth; x++ )
        {
            if( pLetterBuffer[ x ] != FONT_CLEAR_COLOR )
            {
                if( pLetterBuffer[ x ] == 0 )
                {
                    pBackBuffer[ x ] = 0;
                }
                else
                {
                    pBackBuffer[ x ] = textColor;
                }
            }
        }

        pBackBuffer += PLATFORM_LCD_WIDTH;
        pLetterBuffer += (mStride / 2);
    }
}
