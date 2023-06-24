
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
        {}
           
        const char* name;
        UINT32 ns;
        UINT32 filteredNs;
    };

    struct Node
    {
        Node()
            : dest(NULL)
            , parent(HashDefault)
            , frame(0)
            , display(0)
            , expanded(false)
        {}

        Value value;
        TreeNode<Node*>* dest;
        NUINT parent;
        UINT32 frame;
        UINT32 display;
        bool expanded;
    };

    typedef TreeNode<ShockProfiler*> ProfilerNode;
    typedef TreeNode<Node*> TreeNode;
    typedef HashTableIterator<NUINT, Node, ShockProfilerCount> HashIterator;

private:
    static bool CaptureNode(void* data, ProfilerNode *node);
    static bool PrintNode(void* data, TreeNode *node);
    static bool BuildDisplay(void* data, TreeNode* treeNode);

private:
    static HashTable<NUINT, Node, ShockProfilerCount> mHash;
    static Tree<Node*, ShockProfilerCount> mTree;
    static Array<Node*, ShockProfilerCount> mDisplay;
    static Node* mSelected;
    static UINT32 mFrame;
};

#define SHOCK_PROFILE_RENDER \
    ShockProfilersDisplay::Render()

#else // SHOCK_PROFILERS
    #define SHOCK_PROFILE_RENDER

#endif // SHOCK_PROFILERS
#endif // SHOCKPROFILERSDISPLAY_H_