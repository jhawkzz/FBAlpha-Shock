
// See License.md for license

#ifndef SHOCKPROFILERDISPLAY_H_
#define SHOCKPROFILERDISPLAY_H_

#include "shock/shock.h"
#include "shock/shockprofiler.h"
#include "shock/util/hash_table.h"

class ShockProfilerDisplay
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
    static void CaptureNode(void* data, TreeNode<ShockProfiler *> *node);

private:
    typedef HashTableIterator<NUINT, ShockProfilerDisplay::Node, ShockProfilerCount> HashIterator;
    static HashTable<NUINT, Node, ShockProfilerCount> mHash;
    static Tree<Value*, ShockProfilerCount> mTree;
    static Array<ShockProfilerDisplay::Node*, ShockProfilerCount> mAdded;
    static UINT32 mFrame;
};

#endif