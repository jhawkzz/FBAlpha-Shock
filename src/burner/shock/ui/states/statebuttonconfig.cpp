
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
        mpButtonImageMap[ P1_Button_4 ] = (UINT16 *)gGrayTButtonASPBytes;
        mpButtonImageMap[ P1_Button_5 ] = (UINT16 *)gBlueButtonBytes;
        mpButtonImageMap[ P1_Button_6 ] = (UINT16 *)gGrayBButtonASPBytes;
        mpButtonImageMap[ P1_Button_7 ] = (UINT16 *)gWhiteTButtonASPBytes;
        mpButtonImageMap[ P1_Button_8 ] = (UINT16 *)gWhiteBButtonASPBytes;
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
            else if ( ShockInput::GetInput( P1_Button_1 )->WasReleased( ) )
            {
                mConfiguringButton = 1;
            }

            // watch for reset defaults
            if ( ActivePlatform_MVSX == gActivePlatform )
            {
                if ( ShockInput::GetInput( P2_InsertCoin )->WasReleased( ) )
                {
                    ShockPlayerInput::SetGameDefaults( );
                }
            }
            else
            {
                // on ASP this is "Select"
                if ( ShockInput::GetInput( P1_InsertCoin )->WasReleased( ) )
                {
                    ShockPlayerInput::SetGameDefaults( );
                }
            }

            // should we exit?
            return UIBaseState::HandleBackButton( );
        }
        // if they're configuring buttons, don't let them leave
        // until they decide or cancel
        else
        {
            int buttonIndex = CheckButtonReleased( );
            if ( buttonIndex > -1 )
            {
                // get the input they're configuring
                ShockButtonToBurnInput *pInputMap = ShockPlayerInput::GetInputMapForPlayer( mPlayerSelection );
                pInputMap->fireButtonLookup[ mButtonSelection ] = (ShockButton)buttonIndex;

                // be done configuring that button
                mConfiguringButton = 0;
            }

            if ( ActivePlatform_MVSX == gActivePlatform )
            {
                if ( ShockInput::GetInput( P1_InsertCoin )->WasReleased( ) )
                {
                    mConfiguringButton = 0;
                }
            }
            else
            {
                if ( ShockInput::GetInput( OptionsMenu )->WasReleased( ) )
                {
                    mConfiguringButton = 0;
                }
            }
        }

        return UIState_Count;
    }
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
        
        if ( mConfiguringButton == 1 )
        {
            UIRenderer::DrawText( "Press the desired button for this input", UI_X_POS_MENU, 700, 0xFFFF );

            if ( gActivePlatform == ActivePlatform_MVSX )
            {
                UIRenderer::DrawText( "Press Options/Back to cancel", UI_X_POS_MENU, 700 + UI_ROW_HEIGHT, 0xFFFF );
            }
            else
            {
                UIRenderer::DrawText( "Press Options to cancel", UI_X_POS_MENU, 700 + UI_ROW_HEIGHT, 0xFFFF );
            }
        }
        else
        {
            UIRenderer::DrawText( "Select input to configure", UI_X_POS_MENU, 700, 0xFFFF );
            UIRenderer::DrawText( "Press Select to Restore Defaults", UI_X_POS_MENU, 700 + UI_ROW_HEIGHT, 0xFFFF );
        }
        
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

int StateButtonConfig::CheckButtonReleased( )
{
    for ( int i = P1_Button_1; i <= P1_Button_8; i++ )
    {
        if ( ShockInput::GetInput( (ShockButton)i )->WasReleased( ) )
        {
            return i;
        }
    }

    for ( int i = P1_Button_2; i <= P1_Button_2; i++ )
    {
        if ( ShockInput::GetInput( (ShockButton)i )->WasReleased( ) )
        {
            return i;
        }
    }

    return -1;
}