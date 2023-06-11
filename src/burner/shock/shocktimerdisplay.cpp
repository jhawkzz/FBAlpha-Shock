#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/shocktimerdisplay.h"

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
};

scTree<ShockTimerDisplay::Value*, TimerCount> ShockTimerDisplay::m_tree;
scHashTable<const char*, ShockTimerDisplay::Node, TimerCount> ShockTimerDisplay::m_hash;

void ShockTimerDisplay::Capture()
{
    scTimerTree::TraverseDepth(NULL, CaptureNode);

    static const float k = .1f;

    for (auto kv = m_hash.Iterator(); kv; ++kv)
    {
        scTreeNode<scTimer*>* s = kv.Val().source;
        scTreeNode<Value*>* d = kv.Val().dest;

        scTimer* timer = s->val;
        d->val->ns = d->val->ns * (1 - k) + (timer->Time() * k);
        d->val->name = timer->Name();
        d->parent = s->parent ? m_hash[s->parent->val->Name()].dest : NULL;
        d->firstChild = s->firstChild ? m_hash[s->firstChild->val->Name()].dest : NULL; 
        d->sibling = s->sibling ? m_hash[s->sibling->val->Name()].dest : NULL;
        d->depth = s->depth;
    }
}

void ShockTimerDisplay::Render()
{
    PrintContext c;
    c.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    c.fontHeight = MET_FONT_LETTER_HEIGHT;
    c.x = 16;
    c.y = 16;

    m_tree.TraverseDepth(&c, PrintNode);
    m_tree.Clear();
}

void ShockTimerDisplay::CaptureNode(void*, scTreeNode<scTimer *> *source)
{
    Node& node = m_hash[source->val->Name()];
    node.source = source;

    if (node.dest)
       return;

    // TODO: Make sure first one allocated from the tree is head
    if (!source->parent)
        node.dest = m_tree.Head();
    else
        node.dest = m_tree.Alloc();

    node.dest->val = &node.value;
}
