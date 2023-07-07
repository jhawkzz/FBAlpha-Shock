
// See License.md for license

#ifndef SHOCKPROFILERDISPLAY_H_
#define SHOCKPROFILERDISPLAY_H_

#include "shock/shock.h"

#ifdef SHOCK_PROFILER

#include "shock/core/ostimer.h"
#include "shock/shockprofiler.h"
#include "shock/util/hash_table.h"

const UINT32 ShockProfilerDisplayInputMs = 1 * MILLI_TO_MICROSECONDS;

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
        {}
           
        const char* name;
        UINT32 ns;
        UINT32 filteredNs;
    };

    struct Entry
    {
        Entry()
            : dest(NULL)
            , parent(HashDefault)
            , frame(0)
            , display(0)
            , expanded(false)
        {}

        Value value;
        TreeNode<Entry*>* dest;
        NUINT parent;
        UINT32 frame;
        UINT32 display;
        bool expanded;
    };

    typedef TreeNode<ShockProfilerNode*> ProfilerNode;
    typedef TreeNode<Entry*> TreeEntry;
    typedef HashTableIterator<NUINT, Entry, ShockProfilerNodeCount> HashIterator;

private:
    static bool CaptureEntry(void* data, ProfilerNode *node);
    static bool PrintEntry(void* data, TreeEntry* node);
    static bool BuildDisplay(void* data, TreeEntry* treeNode);

private:
    static HashTable<NUINT, Entry, ShockProfilerNodeCount> mHash;
    static Tree<Entry*, ShockProfilerNodeCount> mTree;
    static Array<Entry*, ShockProfilerNodeCount> mDisplay;
    static OSTimer mFocusTimer;
    static Entry* mSelected;
    static UINT32 mFrame;
    static bool mPopFocusOnRelease;
};

#define SHOCK_PROFILE_RENDER \
    ShockProfilerDisplay::Render()

#else // SHOCK_PROFILER
    #define SHOCK_PROFILE_RENDER

#endif // SHOCK_PROFILER
#endif // SHOCKPROFILERSDISPLAY_H_