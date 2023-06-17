
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
    typedef HashTableIterator<NUINT, ShockTimerDisplay::Node, TimerCount> HashIterator;
    static HashTable<NUINT, Node, TimerCount> mHash;
    static Tree<Value*, TimerCount> mTree;
    static Array<ShockTimerDisplay::Node*, TimerCount> mAdded;
    static UINT32 mFrame;
};

#endif