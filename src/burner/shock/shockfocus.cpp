#include "shock/shockfocus.h"

Stack<ShockFocusId, ShockFocusMax> ShockFocus::m_stack;

ShockFocusId ShockFocus::Top( )
{
    return m_stack.Top();
}

void ShockFocus::Pop( )
{
    return m_stack.Pop();
}

void ShockFocus::Push( ShockFocusId id )
{
    m_stack.Push(id);
}

