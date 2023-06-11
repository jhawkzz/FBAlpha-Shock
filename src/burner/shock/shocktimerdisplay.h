
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

public:
    struct Value
    {
        Value()
            : name(NULL)
            , ns(0)
            , expanded(false)
        {}
           
        const char* name;
        UINT32 ns;
        bool expanded;
    };

private:
    static void CaptureNode(void* data, scTreeNode<scTimer *> *node);

private:
    static scTree<Value, TimerCount> m_tree;
};

#endif