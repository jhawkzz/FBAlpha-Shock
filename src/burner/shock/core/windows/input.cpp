
// See License.md for license

#include "../../includes.h"

ButtonState Input::mButtonState[ Button_Count ];
int         Input::mInputFileHandle;
int         Input::mThreadRunning;

int Input::Create( )
{
    return -1;
}

void Input::Destroy( )
{
}

void *Input::PollInput_ThreadProc(void *data)
{
    return NULL;
}

void Input::ReadInputs( )
{
}

int Input::GetValueForInput( InputCodeToButtonMapping input )
{
    return 0;
}
