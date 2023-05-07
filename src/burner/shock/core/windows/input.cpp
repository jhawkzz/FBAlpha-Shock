
// See License.md for license

#include "../../includes.h"

int Input::Create( )
{
    return 0;
}

void Input::Destroy( )
{
}

int Input::GetValueForInput( InputCodeToButtonMapping input )
{
    SHORT s = GetKeyState(input);
    return s & 8000 ? 1 : 0;
}
