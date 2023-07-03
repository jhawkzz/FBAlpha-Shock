
#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/input/shockinput.h"
#include "shock/shockprofilersdisplay.h"
#include "shock/util/hash.h"

#ifdef SHOCK_PROFILERS

namespace
{
    struct PrintContext
    {
        int fontWidth;
        int fontHeight;
        int x;
        int y;

        char str[256];
    };

    NUINT RecurseHash(TreeNode<ShockProfiler*>* node, NUINT seed)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), seed)) : seed;
    }

    NUINT RecurseHash(TreeNode<ShockProfiler*>* node)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), HashDefault)) : HashDefault;
    }
};

HashTable<NUINT, ShockProfilersDisplay::Entry, ShockProfilerCount> ShockProfilersDisplay::mHash;
Tree<ShockProfilersDisplay::Entry*, ShockProfilerCount> ShockProfilersDisplay::mTree;
ShockProfilersDisplay::Entry* ShockProfilersDisplay::mSelected;
Array<ShockProfilersDisplay::Entry*, ShockProfilerCount> ShockProfilersDisplay::mDisplay;
UINT32 ShockProfilersDisplay::mFrame;

void ShockProfilersDisplay::Capture()
{
    Array<Entry*, ShockProfilerCount> added;
    ShockProfilers::TraverseDepth(&added, CaptureEntry);

    // associate all the tree nodes with each other
    for (UINT32 i = 0; i < added.Size(); i++)
    {
        Entry& entry = *added[i];

        TreeEntry* d = entry.dest;
        TreeEntry* parent = (entry.parent != HashDefault) ? mHash[entry.parent].dest : NULL;

        if (parent)
            parent->AddChild(d);
    }

    mDisplay.Clear();
    mTree.TraverseDepth(NULL, BuildDisplay);

    for (HashIterator iterator = mHash.Iterator(); iterator; ++iterator)
    {
        Entry& entry = iterator.Val();
        Value& value = entry.value;
        
        UINT32 curNs = entry.frame == mFrame ? value.ns : 0;

        static const float k = .1f;
        value.filteredNs = UINT32(value.filteredNs * (1 - k) + (curNs * k));
    }

    if (!mDisplay.Size()) // no timers
        return;

    if ( ShockInput::GetInput( P1_Joy_Down )->WasReleased() )
    {
        UINT32 next = mSelected->display + 1;
        mSelected = next < mDisplay.Size() ? mDisplay[next] : mSelected;
    }
    else if ( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        UINT32 prev = mSelected->display - 1;
        mSelected = prev < mDisplay.Size() ? mDisplay[prev] : mSelected;
    }
    else if ( ShockInput::GetInput( P1_Joy_Right )->WasPressed() )
        mSelected->expanded = true;
    else if ( ShockInput::GetInput( P1_Joy_Left )->WasPressed() )
        mSelected->expanded = false;
    
    ++mFrame;
}

void ShockProfilersDisplay::Render()
{
    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    for (UINT32 i = 0; i < mDisplay.Size(); i++)
        PrintEntry(&c, mDisplay[i]->dest);
}

bool ShockProfilersDisplay::CaptureEntry(void* data, ProfilerNode *source)
{
    NUINT hash = RecurseHash(source);

    Entry& entry = mHash[hash];
    entry.parent = RecurseHash(source->parent);
    entry.frame = mFrame;

    ShockProfiler* profiler = source->val;

    Value& value = entry.value;
    value.name = profiler->Name();
    value.ns = profiler->Time();

    if (!entry.dest)
    {
        Array<Entry*, ShockProfilerCount>* added = (Array<Entry*, ShockProfilerCount>*) data;

        entry.dest = !source->parent ? mTree.Head() : mTree.Alloc();
        entry.dest->val = &entry;
        added->Append(&entry);
    }

    return true;
}

bool ShockProfilersDisplay::PrintEntry(void* data, TreeEntry* treeEntry)
{
    TreeEntry* parent = treeEntry->parent;
        
    if (treeEntry->parent && !treeEntry->parent->val->expanded)
        return false;

    Entry& entry = *treeEntry->val;

    PrintContext* c = (PrintContext*) data;

    snprintf( c->str, sizeof( c->str ), " %c%s %dms", 
        entry.expanded ? ',' : '.', 
        entry.value.name, 
        entry.value.filteredNs / 1000 );
    Font::Print( c->str, c->x + treeEntry->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFFu );

    if (mSelected == &entry)
        Font::Print( "X", c->x + treeEntry->depth * MET_FONT_LETTER_WIDTH, c->y, 0XFFEAu );

    c->y += c->fontHeight;

    return true;
}

bool ShockProfilersDisplay::BuildDisplay(void* data, TreeEntry* treeEntry)
{
    TreeEntry* parent = treeEntry->parent;

    if (treeEntry->parent && !treeEntry->parent->val->expanded)
        return false;

    Entry& entry = *treeEntry->val;

    entry.display = mDisplay.Size();
    mDisplay.Append(&entry);

    if (!mSelected)
        mSelected = &entry;

    return true;
}

#endif // SHOCK_PROFILERS
