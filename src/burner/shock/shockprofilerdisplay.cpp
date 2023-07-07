
#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/input/shockinput.h"
#include "shock/shockfocus.h"
#include "shock/shockprofilerdisplay.h"
#include "shock/util/hash.h"

#ifdef SHOCK_PROFILER

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

    NUINT RecurseHash(TreeNode<ShockProfilerNode*>* node, NUINT seed)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), seed)) : seed;
    }

    NUINT RecurseHash(TreeNode<ShockProfilerNode*>* node)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), HashDefault)) : HashDefault;
    }
};

HashTable<NUINT, ShockProfilerDisplay::Entry, ShockProfilerNodeCount> ShockProfilerDisplay::mHash;
Tree<ShockProfilerDisplay::Entry*, ShockProfilerNodeCount> ShockProfilerDisplay::mTree;
OSTimer ShockProfilerDisplay::mFocusTimer;
ShockProfilerDisplay::Entry* ShockProfilerDisplay::mSelected;
Array<ShockProfilerDisplay::Entry*, ShockProfilerNodeCount> ShockProfilerDisplay::mDisplay;
UINT32 ShockProfilerDisplay::mFrame;
bool ShockProfilerDisplay::mPopFocusOnRelease;

void ShockProfilerDisplay::Capture()
{
    Array<Entry*, ShockProfilerNodeCount> added;
    ShockProfiler::TraverseDepth(&added, CaptureEntry);

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

    if (!mDisplay.Size()) // no profiler nodes
        return;

    if ( ShockInput::GetInput( P1_Start )->WasPressed() )
    {
        mFocusTimer.Reset();
    }
    else if ( ShockInput::GetInput( P1_Start )->GetState() == InputState_Pressed)
    {
        // go to focus if P1_Start is held down for N seconds
        if (mFocusTimer.GetElapsedTimeMilliseconds() >= ShockProfilerDisplayInputMs &&
            ShockFocus::Top() != ShockFocusProfilerDisplayId)
        {
            ShockFocus::Push(ShockFocusProfilerDisplayId);
        }
    }
    else if ( ShockInput::GetInput( P1_Start )->WasReleased() )
    {
        if (mPopFocusOnRelease && ShockFocus::Top() == ShockFocusProfilerDisplayId)
        {
            ShockFocus::Pop();
        }

        // First released will set this to true so next release
        // will pop the focus in the conditional above
        mPopFocusOnRelease = !mPopFocusOnRelease;
    }

    if ( ShockFocus::Top() == ShockFocusProfilerDisplayId )
    {
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
    }

    ++mFrame;
}

void ShockProfilerDisplay::Render()
{
    int w, h;
    FrameBuffer::GetSize(&w, &h);
    Font::SetRenderBuffer( FrameBuffer::GetBackBuffer( ), w, h );

    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    for (UINT32 i = 0; i < mDisplay.Size(); i++)
        PrintEntry(&c, mDisplay[i]->dest);
}

bool ShockProfilerDisplay::CaptureEntry(void* data, ProfilerNode *source)
{
    NUINT hash = RecurseHash(source);

    Entry& entry = mHash[hash];
    entry.parent = RecurseHash(source->parent);
    entry.frame = mFrame;

    ShockProfilerNode* profiler = source->val;

    Value& value = entry.value;
    value.name = profiler->Name();
    value.ns = profiler->Time();

    if (!entry.dest)
    {
        Array<Entry*, ShockProfilerNodeCount>* added = (Array<Entry*, ShockProfilerNodeCount>*) data;

        entry.dest = !source->parent ? mTree.Head() : mTree.Alloc();
        entry.dest->val = &entry;
        added->Append(&entry);
    }

    return true;
}

bool ShockProfilerDisplay::PrintEntry(void* data, TreeEntry* treeEntry)
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

    if (mSelected == &entry && ShockFocus::Top() == ShockFocusProfilerDisplayId )
    {
        Font::Print( "X", c->x + treeEntry->depth * MET_FONT_LETTER_WIDTH, c->y, 0XFFEAu );
    }

    c->y += c->fontHeight;

    return true;
}

bool ShockProfilerDisplay::BuildDisplay(void* data, TreeEntry* treeEntry)
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

#endif // SHOCK_PROFILER
