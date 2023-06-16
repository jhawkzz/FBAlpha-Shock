
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
            , filteredNs(0)
            , expanded(false)
        {}
           
        const char* name;
        UINT32 ns;
        UINT32 filteredNs;
        bool expanded;
    };

    struct Node
    {
        Node()
            : dest(NULL)
            , parent(HashDefault)
        {}

        Value value;
        TreeNode<Value*>* dest;
        NUINT parent;
        UINT32 frame;
    };

private:
    static void CaptureNode(void* data, TreeNode<Timer *> *node);

private:
    static HashTable<NUINT, Node, TimerCount> m_hash;
    static Tree<Value*, TimerCount> m_tree;
    static Array<ShockTimerDisplay::Node*, TimerCount> m_added;
    static UINT32 m_frame;
};

#endif