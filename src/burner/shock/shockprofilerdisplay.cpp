#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/input/shockinput.h"
#include "shock/shockprofilerdisplay.h"
#include "shock/util/hash.h"

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

HashTable<NUINT, ShockProfilerDisplay::Node, ShockProfilerCount> ShockProfilerDisplay::mHash;
Tree<ShockProfilerDisplay::Value*, ShockProfilerCount> ShockProfilerDisplay::mTree;
Array<ShockProfilerDisplay::Node*, ShockProfilerCount> ShockProfilerDisplay::mAdded;
TreeNode<ShockProfilerDisplay::Value*>* ShockProfilerDisplay::mSelected;
UINT32 ShockProfilerDisplay::mFrame;

void ShockProfilerDisplay::Capture()
{
    ShockProfilerTree::TraverseDepth(NULL, CaptureNode);

    // associate all the tree nodes with each other
    for (UINT32 i = 0; i < mAdded.Size(); i++)
    {
        Node& node = *mAdded[i];

        TreeNode<Value*>* d = node.dest;
        TreeNode<Value*>* parent = (node.parent != HashDefault) ? mHash[node.parent].dest : NULL;
        if (!parent)
        {
            if (!mSelected )
                mSelected = d;
            continue;
        }

        parent->AddChild(d);
    }

    mAdded.Clear();

    for (HashIterator iterator = mHash.Iterator(); iterator; ++iterator)
    {
        Node& node = iterator.Val();
        Value& value = node.value;
        
        UINT32 curNs = node.frame == mFrame ? value.ns : 0;

        static const float k = .1f;
        value.filteredNs = UINT32(value.filteredNs * (1 - k) + (curNs * k));
    }

    if ( ShockInput::GetInput( P1_Joy_Down )->WasReleased() )
    {
        if ( mSelected->val->expanded && mSelected->firstChild )
            mSelected = mSelected->firstChild;
        else if ( mSelected->nextSibling )
            mSelected = mSelected->nextSibling;
    }
    else if ( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        if ( mSelected->parent && !mSelected->prevSibling )
            mSelected = mSelected->parent;
        else if ( mSelected->prevSibling )
            mSelected = mSelected->prevSibling;
    }
    else if ( ShockInput::GetInput( P1_Joy_Right )->WasPressed() )
        mSelected->val->expanded = true;
    else if ( ShockInput::GetInput( P1_Joy_Left )->WasPressed() )
        mSelected->val->expanded = false;
    
    ++mFrame;
}

void ShockProfilerDisplay::Render()
{
    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    mTree.TraverseDepth(&c, PrintNode);
}

bool ShockProfilerDisplay::CaptureNode(void*, TreeNode<ShockProfiler *> *source)
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
        node.dest->val = &value;
        mAdded.Append(&node);
    }

    return true;
}

bool ShockProfilerDisplay::PrintNode(void* data, TreeNode<ShockProfilerDisplay::Value*>* node)
{
    TreeNode<ShockProfilerDisplay::Value*>* parent = node->parent;
        
    if (node->parent && !node->parent->val->expanded)
        return false;

    Value& value = *node->val;

    PrintContext* c = (PrintContext*) data;

    snprintf( c->str, sizeof( c->str ), " %c%s %dms", 
        value.expanded ? ',' : '.', 
        value.name, 
        value.filteredNs / 1000 );
    Font::Print( c->str, c->x + node->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFFu );

    if (mSelected == node)
        Font::Print( "X", c->x + node->depth * MET_FONT_LETTER_WIDTH, c->y, 0XFFEAu );

    c->y += c->fontHeight;

    return true;
}
