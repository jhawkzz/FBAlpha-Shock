
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/ui/imagebinaries.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/statebuttonconfig.h"

void StateButtonConfig::Create( )
{
    UIBaseState::Create( );
    
    mConfiguringButton     = 0;
    mPlayerSelection       = 0;
    mButtonSelection       = 0;
    mButtonConfigAvailable = 0;
    mpSelectedItem         = NULL;
    
    memset( mNumButtonsPerPlayer, 0, sizeof( mNumButtonsPerPlayer ) );
    memset( mButtonInputList    , 0, sizeof( mButtonInputList ) );
    
    memset( mpButtonImageMap    , 0, sizeof( mpButtonImageMap ) );
    
    // setup an image map
    if ( ActivePlatform_ASP == gActivePlatform )
    {
        // for ASP we're only supporting the built-in buttons (no external controllers yet)
        mpButtonImageMap[ P1_Button_1 ] = (UINT16 *)gRedButtonBytes;
        mpButtonImageMap[ P1_Button_2 ] = (UINT16 *)gYellowButtonBytes;
        mpButtonImageMap[ P1_Button_3 ] = (UINT16 *)gGreenButtonBytes;
        mpButtonImageMap[ P1_Button_4 ] = (UINT16 *)gBlueButtonBytes;
        mpButtonImageMap[ P1_Button_5 ] = (UINT16 *)gWhiteTButtonASPBytes;
        mpButtonImageMap[ P1_Button_6 ] = (UINT16 *)gWhiteBButtonASPBytes;
        mpButtonImageMap[ P1_Button_7 ] = (UINT16 *)gGrayTButtonASPBytes;
        mpButtonImageMap[ P1_Button_8 ] = (UINT16 *)gGrayBButtonASPBytes;
    }
    else
    {
        // for all other platforms do the MVSX layout
        mpButtonImageMap[ P1_Button_1 ] = (UINT16 *)gRedButtonBytes;
        mpButtonImageMap[ P1_Button_2 ] = (UINT16 *)gYellowButtonBytes;
        mpButtonImageMap[ P1_Button_3 ] = (UINT16 *)gGreenButtonBytes;
        mpButtonImageMap[ P1_Button_4 ] = (UINT16 *)gBlueButtonBytes;
        mpButtonImageMap[ P1_Button_5 ] = (UINT16 *)gWhiteMButtonMVSXBytes;
        mpButtonImageMap[ P1_Button_6 ] = (UINT16 *)gWhiteRButtonMVSXBytes;

        mpButtonImageMap[ P2_Button_1 ] = (UINT16 *)gRedButtonBytes;
        mpButtonImageMap[ P2_Button_2 ] = (UINT16 *)gYellowButtonBytes;
        mpButtonImageMap[ P2_Button_3 ] = (UINT16 *)gGreenButtonBytes;
        mpButtonImageMap[ P2_Button_4 ] = (UINT16 *)gBlueButtonBytes;
        mpButtonImageMap[ P2_Button_5 ] = (UINT16 *)gWhiteMButtonMVSXBytes;
        mpButtonImageMap[ P2_Button_6 ] = (UINT16 *)gWhiteRButtonMVSXBytes;
    }
}

void StateButtonConfig::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateButtonConfig::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
    
    mConfiguringButton     = 0;
    mPlayerSelection       = 0;
    mButtonSelection       = 0;
    mButtonConfigAvailable = 0;
    
    // reset our output
    memset( mNumButtonsPerPlayer, 0, sizeof( mNumButtonsPerPlayer ) );
    memset( mButtonInputList    , 0, sizeof( mButtonInputList ) );
    
    int xPos      = UI_X_POS_MENU;
    int yStartPos = UI_Y_POS_MENU + 20; // button graphics are too large to start at the normal offset
    
    int yPos = yStartPos;
    int numPlayers = ShockBurnInput::GetNumPlayers();
    int i;
    for( i = 0; i < min( MAX_SHOCK_PLAYERS, numPlayers ); i++ )
    {
        PlayerInp *pPlayerInput = ShockBurnInput::GetPlayerInput( i );
        
        int c = 0;
        for( c = 0; c < GameInp_Fire_Count; c++ )
        {
            if( pPlayerInput->fireInput[ c ].pGameInp != NULL )
            {
                mButtonInputList[ i ][ c ].Create( pPlayerInput->fireInput[ c ].inputDisplayName, xPos, yPos, 0xFFFF );
                yPos += UI_ROW_HEIGHT + 10;
                
                mNumButtonsPerPlayer[ i ]++;
                
                // flag that this game DOES have input to configure
                mButtonConfigAvailable = 1;
            }
        }
            
        xPos += 400;
        yPos = yStartPos;
    }    
}

void StateButtonConfig::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateButtonConfig::Update( )
{
    UIBaseState::Update( );
    
    DrawMenu( );
    
    // check for menu navigation if not setting a button
    if ( mButtonConfigAvailable == 1 )
    {
        if( mConfiguringButton == 0 )
        {
            if( ShockInput::GetInput( P1_Joy_Down )->WasReleased() )
            {
                mButtonSelection = min( mButtonSelection + 1, mNumButtonsPerPlayer[ mPlayerSelection ] - 1 );
            }
            else if( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
            {
                mButtonSelection = max( mButtonSelection - 1, 0 );
            }
            
            if( ShockInput::GetInput( P1_Joy_Right )->WasReleased() )
            {
                mPlayerSelection = 1;
            }
            else if( ShockInput::GetInput( P1_Joy_Left )->WasReleased() )
            {
                mPlayerSelection = 0;
            }
        }
        else
        {
            // cycle the button for this input
            if( ShockInput::GetInput( P1_Joy_Left )->WasReleased() )
            {
                // get the index to the OS input currently mapped to this game input
                ShockButtonToBurnInput *pInputMap = ShockPlayerInput::GetInputMapForPlayer( mPlayerSelection );
                int buttonIndex = pInputMap->fireButtonLookup[ mButtonSelection ];
                
                // go back to the prior input
                int prevIndex = GetPrevButtonInput( (ShockButton)buttonIndex );
                pInputMap->fireButtonLookup[ mButtonSelection ] = (ShockButton)prevIndex;
            }
            else if( ShockInput::GetInput( P1_Joy_Right )->WasReleased() )
            {
                // get the index to the OS input currently mapped to this game input
                ShockButtonToBurnInput *pInputMap = ShockPlayerInput::GetInputMapForPlayer( mPlayerSelection );
                int buttonIndex = pInputMap->fireButtonLookup[ mButtonSelection ];
                
                // advance to the next input
                int nextIndex = GetNextButtonInput( (ShockButton)buttonIndex );
                pInputMap->fireButtonLookup[ mButtonSelection ] = (ShockButton)nextIndex;
            }
        }

        if( ShockInput::GetInput( P1_Button_1 )->WasReleased() )
        {
            mConfiguringButton = !mConfiguringButton;
        }
    }
    
    if( ShockInput::GetInput( P2_InsertCoin )->WasReleased( ) )
    {
        ShockPlayerInput::SetGameDefaults( );
    }
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateButtonConfig::DrawMenu( )
{
    UIBaseState::RenderTitle( "BUTTON CONFIG" );
    
    if( mButtonConfigAvailable == 1 )
    {
        int i = 0;
        for( i = 0; i < MAX_SHOCK_PLAYERS; i++ )
        {
            for( int c = 0; c < mNumButtonsPerPlayer[ i ]; c++ )
            {
                // if we're config'ing a button, and drawing the one thats 
                // active, color its label green
                if( mConfiguringButton == 1 
                    && i == mPlayerSelection 
                    && c == mButtonSelection )
                {
                    mButtonInputList[ i ][ c ].SetColor( UI_COLOR_ENABLED );
                }
                else
                {
                    mButtonInputList[ i ][ c ].SetColor( 0xFFFF );
                }
            
                mButtonInputList[ i ][ c ].Draw( );
                
                // draw the associated button to the right
                ShockButtonToBurnInput *pInputMap = ShockPlayerInput::GetInputMapForPlayer( i );
                int buttonIndex = pInputMap->fireButtonLookup[ c ];
                
                int xPos = mButtonInputList[ i ][ c ].GetXPos( ) + UI_CURSOR_X_OFFSET + 225;
                int yPos = mButtonInputList[ i ][ c ].GetYPos( ) - 25;
                
                if( mpButtonImageMap[ buttonIndex ] != NULL )
                {
                    UIRenderer::DrawTransparentSprite( mpButtonImageMap[ buttonIndex ], xPos, yPos, BUTTON_WIDTH, BUTTON_HEIGHT, 0 );
                }
            }
        }
        

        UIRenderer::DrawText( "Select input and press left/right to cycle thru buttons", UI_X_POS_MENU, 600, 0xFFFF );
        UIRenderer::DrawText( "Press Select to Restore Defaults", UI_X_POS_MENU, 600 + UI_ROW_HEIGHT, 0xFFFF );
        
        UIBaseState::RenderMenuCursor( mButtonInputList[ mPlayerSelection ][ mButtonSelection ].GetXPos( ), 
                                       mButtonInputList[ mPlayerSelection ][ mButtonSelection ].GetYPos( ) );
    }
    // games like RockClim don't seem to have buttons to configure
    else
    {
        char noConfigStr[ MAX_PATH ] = { 0 };
        strncpy( noConfigStr, "No Buttons Available to Configure", sizeof( noConfigStr ) - 1 );
        int xPos = UIBaseState::GetCenteredXPos( noConfigStr );
        
        UIRenderer::DrawText( noConfigStr, xPos, UI_Y_POS_MENU, UI_COLOR_DISABLED );
    }
    
    UIBaseState::RenderBackOption( "Return" );
}

int StateButtonConfig::GetPrevButtonInput( ShockButton buttonIndex )
{
    int prevIndex = buttonIndex - 1;

    // only ASP should be able to access the 7th & 8th buttons (mvsx doesn't have them)
    ShockButton p1HighButton = ActivePlatform_ASP == gActivePlatform ? P1_Button_8 : P1_Button_6;
    ShockButton p2HighButton = ActivePlatform_ASP == gActivePlatform ? P2_Button_8 : P2_Button_6;

    // are we in P1 range or P2?
    if ( buttonIndex >= P1_Button_1 && buttonIndex <= p1HighButton )
    {
        if ( prevIndex < P1_Button_1 ) prevIndex = p1HighButton;
    }
    else if ( buttonIndex >= P2_Button_1 && buttonIndex <= p2HighButton )
    {
        if ( prevIndex < P2_Button_1 ) prevIndex = p2HighButton;
    }
    
    return prevIndex;
}

int StateButtonConfig::GetNextButtonInput( ShockButton buttonIndex )
{
    int nextIndex = buttonIndex + 1;

    // only ASP should be able to access the 7th & 8th buttons (mvsx doesn't have them)
    ShockButton p1HighButton = ActivePlatform_ASP == gActivePlatform ? P1_Button_8 : P1_Button_6;
    ShockButton p2HighButton = ActivePlatform_ASP == gActivePlatform ? P2_Button_8 : P2_Button_6;

    // are we in P1 range or P2?
    if ( buttonIndex >= P1_Button_1 && buttonIndex <= p1HighButton )
    {
        if ( nextIndex > p1HighButton ) nextIndex = P1_Button_1;
    }
    else if ( buttonIndex >= P2_Button_1 && buttonIndex <= p2HighButton )
    {
        if ( nextIndex > p2HighButton ) nextIndex = P2_Button_1;
    }

    return nextIndex;
}
