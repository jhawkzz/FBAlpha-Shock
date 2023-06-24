
// See License.md for license

#ifndef SHOCKPROFILERSDISPLAY_H_
#define SHOCKPROFILERSDISPLAY_H_

#include "shock/shock.h"

#ifdef SHOCK_PROFILERS

#include "shock/shockprofilers.h"
#include "shock/util/hash_table.h"

class ShockProfilersDisplay
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
    static bool CaptureNode(void* data, TreeNode<ShockProfiler *> *node);
    static bool PrintNode(void* data, TreeNode<Value*>* node);

private:
    typedef HashTableIterator<NUINT, ShockProfilersDisplay::Node, ShockProfilerCount> HashIterator;
    static HashTable<NUINT, Node, ShockProfilerCount> mHash;
    static Tree<Value*, ShockProfilerCount> mTree;
    static Array<ShockProfilersDisplay::Node*, ShockProfilerCount> mAdded;
    static TreeNode<Value*>* mSelected;
    static UINT32 mFrame;
};

#define SHOCK_PROFILE_RENDER \
    ShockProfilersDisplay::Render()

#else // SHOCK_PROFILERS
    #define SHOCK_PROFILE_RENDER

#endif // SHOCK_PROFILERS
#endif // SHOCKPROFILERSDISPLAY_H_