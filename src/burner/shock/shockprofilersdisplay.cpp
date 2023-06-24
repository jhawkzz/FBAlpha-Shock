
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

    //TreeNode<ShockProfiler*>* FindLastParentSibling(TreeNode<ShockProfiler*>* node)
    //{
    //    if (node->parent)

    //    if (!node->parent)
    //        return node;

    //}

};

HashTable<NUINT, ShockProfilersDisplay::Node, ShockProfilerCount> ShockProfilersDisplay::mHash;
Tree<ShockProfilersDisplay::Node*, ShockProfilerCount> ShockProfilersDisplay::mTree;
Array<ShockProfilersDisplay::Node*, ShockProfilerCount> ShockProfilersDisplay::mAdded;
ShockProfilersDisplay::Node* ShockProfilersDisplay::mSelected;
Array<ShockProfilersDisplay::Node*, ShockProfilerCount> ShockProfilersDisplay::mDisplay;
UINT32 ShockProfilersDisplay::mFrame;

void ShockProfilersDisplay::Capture()
{
    ShockProfilers::TraverseDepth(NULL, CaptureNode);

    // associate all the tree nodes with each other
    for (UINT32 i = 0; i < mAdded.Size(); i++)
    {
        Node& node = *mAdded[i];

        TreeNode* d = node.dest;
        TreeNode* parent = (node.parent != HashDefault) ? mHash[node.parent].dest : NULL;

        if (parent)
            parent->AddChild(d);
    }

    mAdded.Clear();

    mDisplay.Clear();
    mTree.TraverseDepth(NULL, BuildDisplay);

    for (HashIterator iterator = mHash.Iterator(); iterator; ++iterator)
    {
        Node& node = iterator.Val();
        Value& value = node.value;
        
        UINT32 curNs = node.frame == mFrame ? value.ns : 0;

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
        PrintNode(&c, mDisplay[i]->dest);
}

bool ShockProfilersDisplay::CaptureNode(void*, ProfilerNode *source)
{
    NUINT hash = RecurseHash(source);

    Node& node = mHash[hash];
    node.parent = RecurseHash(source->parent);
    node.frame = mFrame;

    ShockProfiler* profiler = source->val;

    Value& value = node.value;
    value.name = profiler->Name();
    value.ns = profiler->Time();

    if (!node.dest)
    {
        node.dest = !source->parent ? mTree.Head() : mTree.Alloc();
        node.dest->val = &node;
        mAdded.Append(&node);
    }

    return true;
}

bool ShockProfilersDisplay::PrintNode(void* data, TreeNode* treeNode)
{
    TreeNode* parent = treeNode->parent;
        
    if (treeNode->parent && !treeNode->parent->val->expanded)
        return false;

    Node& node = *treeNode->val;

    PrintContext* c = (PrintContext*) data;

    snprintf( c->str, sizeof( c->str ), " %c%s %dms", 
        node.expanded ? ',' : '.', 
        node.value.name, 
        node.value.filteredNs / 1000 );
    Font::Print( c->str, c->x + treeNode->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFFu );

    if (mSelected == &node)
        Font::Print( "X", c->x + treeNode->depth * MET_FONT_LETTER_WIDTH, c->y, 0XFFEAu );

    c->y += c->fontHeight;

    return true;
}

bool ShockProfilersDisplay::BuildDisplay(void* data, TreeNode* treeNode)
{
    TreeNode* parent = treeNode->parent;

    if (treeNode->parent && !treeNode->parent->val->expanded)
        return false;

    Node& node = *treeNode->val;

    node.display = mDisplay.Size();
    mDisplay.Append(&node);

    if (!mSelected)
        mSelected = &node;

    return true;
}

#endif // SHOCK_PROFILERS
