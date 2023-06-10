
// See License.md for license

#ifndef SHOCKTIMERDISPLAY_H_
#define SHOCKTIMERDISPLAY_H_

#include "shock/shock.h"
#include "shock/shockprofiler.h"
#include "shock/util/hash_table.h"

class ShockTimerDisplay
{
public:
    static void Capture( );
    static void Render( );

    static void Capture(scTreeNode<scTimer *> *node);

private:
    struct Value
    {
        UINT32 depth;
        UINT32 ns;
    };

    static scHashTable<const char*, Value, TimerCount> m_timers;
};

#endif