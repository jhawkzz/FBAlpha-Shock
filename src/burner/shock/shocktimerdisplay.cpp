#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/shocktimerdisplay.h"

namespace
{
    
    struct CaptureContext
    {
        scHashTable<scTreeNode<scTimer*>*, scTreeNode<ShockTimerDisplay::Value>*, TimerCount> hash;
    };

    struct PrintContext
    {
        int fontWidth;
        int fontHeight;
        int x;
        int y;

        char str[256];
    };

    void PrintNode(void* data, scTreeNode<ShockTimerDisplay::Value>* node)
    {
        PrintContext* c = (PrintContext*) data;

        snprintf( c->str, sizeof( c->str ), "%s %dms", node->val.name, node->val.ns / 1000 );
        Font::Print( c->str, c->x + node->depth * MET_FONT_LETTER_WIDTH, c->y, 0xFFFF );

        c->y += c->fontHeight;
    }
};

scTree<ShockTimerDisplay::Value, TimerCount> ShockTimerDisplay::m_tree;

void ShockTimerDisplay::Capture()
{
    CaptureContext c;
    scTimerTree::TraverseDepth(&c, CaptureNode);

    static const float k = .1f;

    for (auto kv = c.hash.Iterator(); kv; ++kv)
    {
        scTreeNode<scTimer*>* s = kv.Key();
        scTreeNode<Value>* d = kv.Val();
        
        scTimer* timer = s->val;
        d->val.ns = d->val.ns * (1 - k) + (timer->Time() * k);
        d->val.name = timer->Name();
        d->parent = c.hash[s->parent];
        d->firstChild = c.hash[s->firstChild];
        d->sibling = c.hash[s->sibling];
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
}

void ShockTimerDisplay::CaptureNode(void* data, scTreeNode<scTimer *> *source)
{
    CaptureContext* c = (CaptureContext*) data;
    scTreeNode<Value>*& node = c->hash[source];
    
    // TODO: Make sure first one allocated from the tree is head
    if (!node)
        node = m_tree.Alloc();
}
