
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
    mpButtonImageMap[ P1_Button_1 ] = (UINT16 *)gRedButtonBytes;
    mpButtonImageMap[ P1_Button_2 ] = (UINT16 *)gYellowButtonBytes;
    mpButtonImageMap[ P1_Button_3 ] = (UINT16 *)gGreenButtonBytes;
    mpButtonImageMap[ P1_Button_4 ] = (UINT16 *)gBlueButtonBytes;
    mpButtonImageMap[ P1_Button_5 ] = (UINT16 *)gWhiteMButtonBytes;
    mpButtonImageMap[ P1_Button_6 ] = (UINT16 *)gWhiteRButtonBytes;
    
    mpButtonImageMap[ P2_Button_1 ] = (UINT16 *)gRedButtonBytes;
    mpButtonImageMap[ P2_Button_2 ] = (UINT16 *)gYellowButtonBytes;
    mpButtonImageMap[ P2_Button_3 ] = (UINT16 *)gGreenButtonBytes;
    mpButtonImageMap[ P2_Button_4 ] = (UINT16 *)gBlueButtonBytes;
    mpButtonImageMap[ P2_Button_5 ] = (UINT16 *)gWhiteMButtonBytes;
    mpButtonImageMap[ P2_Button_6 ] = (UINT16 *)gWhiteRButtonBytes;
    
    mRestoreDefaults.Create( "Press P2 Start to Restore Defaults", UI_X_POS_MENU, 600, 0xFFFF );
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
    
    if( ShockInput::GetInput( P2_Start )->WasReleased( ) )
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
        
        mRestoreDefaults.Draw( );
        
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
    /*switch( buttonIndex )
    {
        case P1_Red:      return P1_RightBot; break;
        case P1_Yellow:   return P1_Red;      break;
        case P1_Green:    return P1_Yellow;   break;
        case P1_Blue:     return P1_Green;    break;
        case P1_MidBot:   return P1_Blue;     break;
        case P1_RightBot: return P1_MidBot;   break;
        
        case P2_Red:      return P2_RightBot; break;
        case P2_Yellow:   return P2_Red;      break;
        case P2_Green:    return P2_Yellow;   break;
        case P2_Blue:     return P2_Green;    break;
        case P2_MidBot:   return P2_Blue;     break;
        case P2_RightBot: return P2_MidBot;   break;
    
        // for out of range buttons, return itself    
        default: return buttonIndex;
    }*/
    
    int prevIndex = buttonIndex - 1;
    if( prevIndex < 0 ) prevIndex = ShockButton_Count - 1;
    
    return prevIndex;
}

int StateButtonConfig::GetNextButtonInput( ShockButton buttonIndex )
{
    return (buttonIndex + 1) % ShockButton_Count;
    /*switch( buttonIndex )
    {
        case P1_Red:      return P1_Yellow;   break;
        case P1_Yellow:   return P1_Green;    break;
        case P1_Green:    return P1_Blue;     break;
        case P1_Blue:     return P1_MidBot;   break;
        case P1_MidBot:   return P1_RightBot; break;
        case P1_RightBot: return P1_Red;      break;
        
        case P2_Red:      return P2_Yellow;   break;
        case P2_Yellow:   return P2_Green;    break;
        case P2_Green:    return P2_Blue;     break;
        case P2_Blue:     return P2_MidBot;   break;
        case P2_MidBot:   return P2_RightBot; break;
        case P2_RightBot: return P2_Red;      break;
    
        // for out of range buttons, return itself    
        default: return buttonIndex;
    }*/
}
