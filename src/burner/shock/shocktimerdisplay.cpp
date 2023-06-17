#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/shocktimerdisplay.h"
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

    void PrintNode(void* data, TreeNode<ShockTimerDisplay::Value*>* node)
    {
        PrintContext* c = (PrintContext*) data;

        snprintf( c->str, sizeof( c->str ), "%s %dms", node->val->name, node->val->filteredNs / 1000 );
        Font::Print( c->str, c->x + node->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFFu );

        c->y += c->fontHeight;
    }

    NUINT RecurseHash(TreeNode<Timer*>* node, NUINT seed)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), seed)) : seed;
    }

    NUINT RecurseHash(TreeNode<Timer*>* node)
    {
        return node ? RecurseHash(node->parent, Hash((NUINT)node->val->Name(), HashDefault)) : HashDefault;
    }
};

HashTable<NUINT, ShockTimerDisplay::Node, TimerCount> ShockTimerDisplay::mHash;
Tree<ShockTimerDisplay::Value*, TimerCount> ShockTimerDisplay::mTree;
Array<ShockTimerDisplay::Node*, TimerCount> ShockTimerDisplay::mAdded;
UINT32 ShockTimerDisplay::mFrame;

void ShockTimerDisplay::Capture()
{
    TimerTree::TraverseDepth(NULL, CaptureNode);

    // associate all the tree nodes with each other
    for (UINT32 i = 0; i < mAdded.Size(); i++)
    {
        Node& node = *mAdded[i];

        TreeNode<Value*>* d = node.dest;
        TreeNode<Value*>* parent = (node.parent != HashDefault) ? mHash[node.parent].dest : NULL;
        if (!parent)
            continue;

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

    ++mFrame;
}

void ShockTimerDisplay::Render()
{
    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    mTree.TraverseDepth(&c, PrintNode);
}

void ShockTimerDisplay::CaptureNode(void*, TreeNode<Timer *> *source)
{
    NUINT hash = RecurseHash(source);

    Node& node = mHash[hash];
    node.parent = RecurseHash(source->parent);
    node.frame = mFrame;

    Timer* timer = source->val;

    Value& value = node.value;
    value.name = timer->Name();
    value.ns = timer->Time();

    if (!node.dest)
    {
        node.dest = !source->parent ? mTree.Head() : mTree.Alloc();
        node.dest->val = &value;

        mAdded.Append(&node);
    }
}
