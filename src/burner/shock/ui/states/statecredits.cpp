
// See License.md for license

#include "shock/includes.h"

void StateCredits::Create( )
{
    UIBaseState::Create( );
    
    mPageNum       = 0;
    mNumMenuItems  = 0;
    mMenuSelection = 0;
    
    mMenuItemList[ mNumMenuItems++ ].Create( "Prev", UI_X_POS_MENU, PLATFORM_LCD_HEIGHT - 100, 0xFFFF );
    mMenuItemList[ mNumMenuItems++ ].Create( "Next", UI_X_POS_MENU + 150, PLATFORM_LCD_HEIGHT - 100, 0xFFFF );
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateCredits::Create() ERROR!!! mNumMenuItems too large at %d." 
                     "MAX_MENU_ITEMS is only: %d\r\n", 
                     mNumMenuItems, 
                     MAX_MENU_ITEMS );
        exit( 0 );
    }
}

void StateCredits::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateCredits::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
    
    mPageNum       = 0;
    mMenuSelection = 1;
}

void StateCredits::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateCredits::Update( )
{
    RenderCredits( );
    
    // check for menu navigation
    if( ShockInput::GetInput( P1_Joy_Left )->WasReleased() )
    {
        mMenuSelection = 0;
    }
    else if( ShockInput::GetInput( P1_Joy_Right )->WasReleased() )
    {
        mMenuSelection = 1;
    }
    
    if( ShockInput::GetInput( P1_Button_1 )->WasReleased() )
    {
        if( mMenuSelection == 0 )
        {
            mPageNum = max( mPageNum - 1, 0 );
        }
        else if ( mMenuSelection == 1 )
        {
            mPageNum = min( mPageNum + 1, NUM_PAGES - 1 );
        }
    }
           
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateCredits::RenderCredits( )
{
    UIBaseState::RenderTitle( "CREDITS / LEGAL" );
    
    switch( mPageNum )
    {
        case 0: RenderPage0( ); break;
        case 1: RenderPage1( ); break;
        case 2: RenderPage2( ); break;
        case 3: RenderPage3( ); break;
        case 4: RenderPage4( ); break;
        case 5: RenderPage5( ); break;
        case 6: RenderPage6( ); break;
    }
    
    UIBaseState::RenderBackOption( "Return" );
    
    int prevTextColor = 0xFFFF;
    int nextTextColor = 0xFFFF;
    if( mPageNum == 0 )
    {
        prevTextColor = UI_COLOR_DISABLED;
    }
    else if ( mPageNum == NUM_PAGES - 1 )
    {
        nextTextColor = UI_COLOR_DISABLED;
    }
    
    mMenuItemList[ 0 ].SetColor( prevTextColor );
    mMenuItemList[ 0 ].Draw( );
    
    mMenuItemList[ 1 ].SetColor( nextTextColor );
    mMenuItemList[ 1 ].Draw( );
    
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ), mMenuItemList[ mMenuSelection ].GetYPos( ) );
}

void StateCredits::RenderPage0( )
{
    char text[ MAX_PATH ] = { 0 };
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    snprintf( text, sizeof( text ) - 1, "Version: %s", SHOCK_VERSION );
    UIRenderer::DrawText( text, xPos, yPos, 0xFFFF );
                          
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Development:", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "100 Mega Shock Edition Port Written by and # 2023 JHawkZZ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Quality Assurance by Usagi", xPos, yPos, 0xFFFF );
                        
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Some text uses the font Upheaval Pro by Brian Kent.", xPos, yPos, 0xFFFF );

    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "DISCLAIMER:", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "The authors of Final Burn Alpha: 100 Mega Shock Edition do not", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "guarantee its fitness for any purpose, implied or otherwise, and", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "do not accept responsibility for any damages whatsoever that might", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "occur when using Final Burn Alpha: 100 Mega Shock Edition. All games", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "emulated, including any images and sounds therein, are copyrighted by", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "their respective copyright holders.", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Final Burn Alpha: 100 Mega Shock Edition DOES NOT INCLUDE any ROM", xPos, yPos, 0xFFFF ); 
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "images of emulated games.", xPos, yPos, 0xFFFF ); 
                    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "License:", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Final Burn Alpha: 100 Mega Shock Edition is a port of", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Final Burn Alpha for the Unico MVSX and is subject to the terms of", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "the Final Burn Alpha license which is as follows:", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "You may freely use, modify, and distribute both the FB Alpha source", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "code and binary, however the following restrictions apply to the ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "FB Alpha original material", xPos, yPos, 0xFFFF );
}

void StateCredits::RenderPage1( )
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "(see below for a list of libraries with differing licenses,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "please consult their respective documentation for more information):", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You may not sell, lease, rent or otherwise seek to gain monetary", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "profit from FB Alpha.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You must make public any changes you make to the source code.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You must include, verbatim, the full text of this license.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You may not distribute binaries which support games with copyright", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "dates less then three years old.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You may not distribute FB Alpha with ROM images unless you have the", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "legal right to distribute them.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You may use dynamic linked libraries (dlls) for network play only", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(all other use is prohibited).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You may not ask for donations to support your work on any project", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "that uses the FB Alpha source code.", xPos, yPos, 0xFFFF );    
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "FB Alpha can currently be obtained from https://www.fbalpha.com.", xPos, yPos, 0xFFFF );
}

void StateCredits::RenderPage2( )
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "FB Alpha would not exist without a lot of code from the MAME project.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "The MAME project is subject to it's own license, which can be found", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "at http://mamedev.org/legal.html", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Due to the use of MAME code in FB Alpha, FB Alpha is also subject ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "to the terms of the MAME license.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "FB Alpha is based on Final Burn", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(formally at http://www.finalburn.com), see additional text below.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Musashi MC68000/MC68010/MC68EC020 CPU core by Karl Stenerud", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;    
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "A68K MC68000 CPU core by Mike Coates & Darren Olafson", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;    
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Z80 CPU core by Juergen Buchmueller (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "ARM CPU core by Bryan McPhail, Phil Stroffolino", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;    
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "ARM7 CPU core by Steve Ellenoff (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "H6280 CPU core by Brian McPhail (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "HD6309 CPU core by John Butler, Tim Lindner (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "I8039 CPU core by Mirko Buffoni (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Konami CPU core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "M6502 CPU core by Juergen Buchmueller (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "M6800/M6801/M6802/M6803/M6808/HD63701/NSC8105 CPU core by MAMEdev", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;    
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "M6805 CPU core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
}

void StateCredits::RenderPage3( )
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "M6809 CPU core by John Bulter (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "NEC V20/V30/V33 CPU core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "PIC16C5X CPU core by Tony La Porta (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "S2650 CPU core by Juergen Buchmueller (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "SH-2 CPU core by Juergen Buchmueller (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "TLCS90 CPU core by Luca Elia (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "ADSP21XX CPU core by Aaron Giles (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "AY8910/YM2149 sound core by various authors (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "C6280 sound core by Charles MacDonald (http://cgfm2.emuviews.com).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "DAC sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "ES8712 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "ICS2115 sound core by O.Galibert, El-Semi (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "IREM GA20 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "K005289 sound core by Brian McPhail (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "K007232 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "K051649 sound core by Brian McPhail (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "K053260 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "K054539 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "MSM5205 sound core by Aaron Giles (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "MSM5232 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "RF5C68 sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "SAA1099 sound core by Juergen Buchmueller, Manuel Abadia", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Sega PCM sound core by MAMEdev (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "SN76496 sound core by Nicola Salmoria (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
}

void StateCredits::RenderPage4()
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "UPD7759 sound core by Juergen Buchmueller, Mike Balfour,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Howie Cohen, Olivier Galibert, Aaron Giles (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "VLM5030 sound core by Tatsuyuki Satoh (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "X1010 sound core by Luca Elia, Manbow-J (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Y8950/YM3526/YM3812 sound core by Jarek Burczynski & Tatsuyuki Satoh", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "YM2151 sound core by Jarek Burczynski (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "YM2203/YM2608/YM2610/YM2612 sound cores by Jarek Burczynski &", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Tatsuyuki Satoh (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "YM2413 sound core by Jarek Burczynski (http://www.mamedev.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "YMF278B sound core by R. Belmont & O.Galibert", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(http://www.mamedev.org).", xPos, yPos, 0xFFFF );
                        
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Uses SMS Plus by Charles MacDonald (http://www.techno-junk.org).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "7Z functionality provided by LZMA SDK (http://www.7-zip.org/sdk.html)", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "PNG functionality provided by libpng (http://www.libpng.org)", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Zip functionality provided by zlib (http://www.zlib.net).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Uses Xbyak (JIT assembler for x86/x64) by Herumi", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(https://github.com/herumi/xbyak)", xPos, yPos, 0xFFFF );
}

void StateCredits::RenderPage5()
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "Some graphics effects provided by the Scale2x, 2xPM, Eagle Graphics,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "2xSaI, hq2x/hq3x/hq4x, hq2xS/hq3xS/SuperEagle/2xSaI (VBA),", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "hq2xS/hq3xS/hq2xBold/hq3xBold/EPXB/EPXC (SNES9X ReRecording) ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "and SuperScale libraries (http://scale2x.sourceforge.net,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "http://2xpm.freeservers.com, http://retrofx.com,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "http://elektron.its.tudelft.nl/~dalikifa, http://www.hiend3d.com,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "http://code.google.com/p/vba-rerecording, ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "http://code.google.com/p/snes9x151-rerecording,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "http://nebula.emulatronia.com", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Miscellaneous other components from various sources.", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Copyright and license information are contained in the relevant parts", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "of the source code.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "All material not covered above * 2004-2017 Team FB Alpha.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "DISCLAIMER:", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "The authors of FB Alpha do not", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "guarantee its fitness for any purpose, implied or otherwise, and", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "do not accept responsibility for any damages whatsoever that might", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "occur when using FB Alpha. All games", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "emulated by FB Alpha, including any images and sounds therein,", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "are copyrighted by their respective copyright holders.", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "FB Alpha DOES NOT INCLUDE any ROM images of emulated games.", xPos, yPos, 0xFFFF );  
}

void StateCredits::RenderPage6()
{
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    UIRenderer::DrawText( "The following information and license conditions accompanied", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "the original Final Burn emulator. They also apply to FB Alpha:", xPos, yPos, 0xFFFF );  
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Copyright (c)2001 Dave (formally of www.finalburn.com), ", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "all rights reserved.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "This refers to all code except where stated otherwise", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "(e.g. unzip and zlib code).", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "You can use, modify and redistribute this code freely as long", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "as you don't do so commercially.", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "This copyright notice must remain with the code. If your", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "program uses this code, you must either distribute or link", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "to the source code.", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "If you modify or improve this code, you must distribute the", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "source code improvements.", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    UIRenderer::DrawText( "Dave", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "Former Homepage: www.finalburn.com", xPos, yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( "E-mail: dave@finalburn.com", xPos, yPos, 0xFFFF );
}
