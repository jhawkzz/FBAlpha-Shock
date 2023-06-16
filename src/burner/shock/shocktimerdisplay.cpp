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

HashTable<NUINT, ShockTimerDisplay::Node, TimerCount> ShockTimerDisplay::m_hash;
Tree<ShockTimerDisplay::Value*, TimerCount> ShockTimerDisplay::m_tree;
Array<ShockTimerDisplay::Node*, TimerCount> ShockTimerDisplay::m_added;
UINT32 ShockTimerDisplay::m_frame;

void ShockTimerDisplay::Capture()
{
    TimerTree::TraverseDepth(NULL, CaptureNode);

    // associate all the tree nodes with each other
    for (UINT32 i = 0; i < m_added.Size(); i++)
    {
        Node& node = *m_added[i];

        TreeNode<Value*>* d = node.dest;
        TreeNode<Value*>* parent = (node.parent != HashDefault) ? m_hash[node.parent].dest : NULL;
        if (!parent)
            continue;

        parent->AddChild(d);
    }

    m_added.Clear();

    for (auto& iterator = m_hash.Iterator(); iterator; ++iterator)
    {
        Node& node = iterator.Val();
        Value& value = node.value;
        
        UINT32 curNs = node.frame == m_frame ? value.ns : 0;

        static const float k = .1f;
        value.filteredNs = UINT(value.filteredNs * (1 - k) + (curNs * k));
    }

    ++m_frame;
}

void ShockTimerDisplay::Render()
{
    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    m_tree.TraverseDepth(&c, PrintNode);
}

void ShockTimerDisplay::CaptureNode(void*, TreeNode<Timer *> *source)
{
    NUINT hash = RecurseHash(source);

    Node& node = m_hash[hash];
    node.parent = RecurseHash(source->parent);
    node.frame = m_frame;

    Timer* timer = source->val;

    Value& value = node.value;
    value.name = timer->Name();
    value.ns = timer->Time();

    if (!node.dest)
    {
        node.dest = !source->parent ? m_tree.Head() : m_tree.Alloc();
        node.dest->val = &value;

        m_added.Append(&node);
    }
}
