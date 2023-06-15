
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
            : dest(NULL)
            , parent(scHashDefault)
        {}

        Value value;
        scTreeNode<Value*>* dest;
        NUINT parent;
    };

private:
    static void CaptureNode(void* data, scTreeNode<scTimer *> *node);

private:
    static scHashTable<NUINT, Node, TimerCount> m_hash;
    static scTree<Value*, TimerCount> m_tree;
    static scArray<ShockTimerDisplay::Node*, TimerCount> m_added;
};

#endif