
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

    typedef TreeNode<ShockProfiler*> ProfilerNode;
    typedef TreeNode<Entry*> TreeEntry;
    typedef HashTableIterator<NUINT, Entry, ShockProfilerCount> HashIterator;

private:
    static bool CaptureEntry(void* data, ProfilerNode *node);
    static bool PrintEntry(void* data, TreeEntry* node);
    static bool BuildDisplay(void* data, TreeEntry* treeNode);

private:
    static HashTable<NUINT, Entry, ShockProfilerCount> mHash;
    static Tree<Entry*, ShockProfilerCount> mTree;
    static Array<Entry*, ShockProfilerCount> mDisplay;
    static Entry* mSelected;
    static UINT32 mFrame;
};

#define SHOCK_PROFILE_RENDER \
    ShockProfilersDisplay::Render()

#else // SHOCK_PROFILERS
    #define SHOCK_PROFILE_RENDER

#endif // SHOCK_PROFILERS
#endif // SHOCKPROFILERSDISPLAY_H_