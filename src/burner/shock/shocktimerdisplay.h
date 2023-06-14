
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

    struct Node
    {
        Node()
            : source(NULL)
            , dest(NULL)
        {}

        Value value;
        scTreeNode<Value*>* dest;
        scTreeNode<scTimer*>* source;
    };

private:
    static void CaptureNode(void* data, scTreeNode<scTimer *> *node);

private:
    static scHashTable<NUINT, Node, TimerCount> m_hash;

};

#endif