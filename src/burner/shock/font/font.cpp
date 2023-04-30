
// See License.md for license

#include "../includes.h"
#include "shock/font/fontbinary.h"

UINT16 *Font::mpFontTexture;
char    Font::mCharLookup[ 128 ];
char    Font::mFont[ FONT_LETTER_COUNT ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
                                             'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
                                             'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ',', '.', '!',
                                             '%', ':', '*', '-', '/' }; //* = Copyright logo
Letter Font::mCharPositionLookup[ FONT_LETTER_COUNT ];

void Font::Create( )
{
    mpFontTexture = (UINT16 *)gFontBytes;
    
    // setup our lookup tables
    int lettersPerRow = FONT_WIDTH / FONT_LETTER_WIDTH;
    
    int i;
    for ( i = 0; i < FONT_LETTER_COUNT; i++ )
    {
        // fill our char position lookup (where the X/Y pixel start for each letter is)
        mCharPositionLookup[ i ].xPos = (i % lettersPerRow) * FONT_LETTER_WIDTH;
        mCharPositionLookup[ i ].yPos = (i / lettersPerRow) * FONT_LETTER_HEIGHT;
        
        mCharLookup[ mFont[ i ] ] = (char) i;

        if ( i < FONT_LETTER_COUNT - 8 )
        {
            // force lowercase to use the same indices
            mCharLookup[ mFont[ i ] + 32 ] = (char) i; 
        }
    }
}

void Font::Print( UINT16 *pBackBuffer, const char *pString, int xPos, int yPos, short textColor )
{
    // clamp height
    yPos = min( yPos, PLATFORM_LCD_HEIGHT - FONT_LETTER_HEIGHT );
    
    // clamp width (by limiting string length)
    int allowedStringLen = strlen( pString );
    
    int maxWidth = PLATFORM_LCD_WIDTH - xPos;
    int stringWidth = (allowedStringLen * FONT_LETTER_FULL_WIDTH) - FONT_SPACING;
    
    // is to too long?
    int delta = stringWidth - maxWidth;
    if( delta > 0 )
    {
        // calculate how many letters are safe to render
        int allowedWidth = stringWidth - delta;
        allowedStringLen = allowedWidth / FONT_LETTER_FULL_WIDTH;
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
                int letter_index = mCharLookup[ pString[ strIndex ] ];
            
                // render the letter to the screen
                UINT16 *pLetter = mpFontTexture 
                                + ( mCharPositionLookup[ letter_index ].yPos * (FONT_STRIDE / 2) ) 
                                +   mCharPositionLookup[ letter_index ].xPos;
                
                RenderLetter( pCurrBuffer, pLetter, textColor );

                break;
            }

            case 32:
            {
                break;
            }
        }

        pCurrBuffer += FONT_LETTER_WIDTH + FONT_SPACING;

        strIndex++;
    }
}

UINT32 Font::MeasureStringWidth( const char *pText )
{
    return strlen( pText ) * FONT_LETTER_FULL_WIDTH;
}

void Font::RenderLetter( UINT16 *pBackBuffer, UINT16 *pLetterBuffer, UINT16 textColor )
{
    int y;
    for ( y = 0; y < FONT_LETTER_HEIGHT; y++ )
    {
        int x;
        for ( x = 0; x < FONT_LETTER_WIDTH; x++ )
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
        pLetterBuffer += (FONT_STRIDE / 2);
    }
}
