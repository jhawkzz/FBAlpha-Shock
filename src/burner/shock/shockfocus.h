#ifndef SHOCKFOCUS_H_
#define SHOCKFOCUS_H_

#include "shock/shock.h"
#include "shock/util/stack.h"

enum ShockFocusId
{
    ShockFocusProfilerDisplayId,
    ShockFocusPlayerInputId,
};

const UINT32 ShockFocusMax = 8;

class ShockFocus
{
public:
    static ShockFocusId Top( );
    static void Pop( );

    static void Push( ShockFocusId id );

private:
    static Stack<ShockFocusId, ShockFocusMax> m_stack;
};

#endif // SHOCKFOCUS_H_
