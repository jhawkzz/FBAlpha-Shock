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

    void PrintNode(void* data, scTreeNode<ShockTimerDisplay::Value*>* node)
    {
        PrintContext* c = (PrintContext*) data;

        snprintf( c->str, sizeof( c->str ), "%s %dms", node->val->name, node->val->ns / 1000 );
        Font::Print( c->str, c->x + node->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFF );

        c->y += c->fontHeight;
    }

    NUINT Hash(scTreeNode<scTimer*>* node, NUINT seed)
    {
        return node ? Hash(node->parent, scHash((NUINT)node->val->Name(), seed)) : seed;
    }

    NUINT Hash(scTreeNode<scTimer*>* node)
    {
        return node ? Hash(node->parent, scHash((NUINT)node->val->Name(), scHashDefault)) : scHashDefault;
    }
};

scHashTable<NUINT, ShockTimerDisplay::Node, TimerCount> ShockTimerDisplay::m_hash;
scTree<ShockTimerDisplay::Value*, TimerCount> ShockTimerDisplay::m_tree;
UINT32 ShockTimerDisplay::m_frame;

void ShockTimerDisplay::Capture()
{
    m_tree.Clear();
    scTimerTree::TraverseDepth(NULL, CaptureNode);

    // associate all the tree nodes with each other
    for (auto kv = m_hash.Iterator(); kv; ++kv)
    {
        Node& node = kv.Val();

        if (node.frame != m_frame)
            continue;

        scTreeNode<scTimer*>* s = node.source;
        scTreeNode<Value*>* d = node.dest;

        d->parent = s->parent ? m_hash[Hash(s->parent)].dest : NULL;
        d->firstChild = s->firstChild ? m_hash[Hash(s->firstChild)].dest : NULL; 
        d->sibling = s->sibling ? m_hash[Hash(s->sibling)].dest : NULL;
        d->depth = s->depth;
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

void ShockTimerDisplay::CaptureNode(void*, scTreeNode<scTimer *> *source)
{
    NUINT hash = Hash(source);

    Node& node = m_hash[hash];
    node.frame = m_frame;
    node.source = source;

    scTimer* timer = source->val;

    Value& value = node.value;
    value.name = timer->Name();

    static const float k = .1f;
    value.ns = UINT(value.ns * (1 - k) + (timer->Time() * k));

    node.dest = !source->parent ? m_tree.Head() : m_tree.Alloc();
    node.dest->val = &value;
}
