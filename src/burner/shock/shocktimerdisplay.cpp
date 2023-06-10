#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/shocktimerdisplay.h"

namespace
{
    void CaptureTime( void *data, scTreeNode<scTimer *> *node )
    {
        ShockTimerDisplay::Capture(node);
    }
};

scHashTable<const char*, ShockTimerDisplay::Value, TimerCount> ShockTimerDisplay::m_timers;

void ShockTimerDisplay::Capture()
{
    scTimerTree::TraverseDepth(NULL, CaptureTime);
}

void ShockTimerDisplay::Render()
{
    int fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    int fontHeight = MET_FONT_LETTER_HEIGHT;
    int x = 16;//fbWidth - context.fontWidth;
    int y = 16;//fbHeight - context.fontHeight;

    int fbWidth;
    int fbHeight;
    FrameBuffer::GetSize( &fbWidth, &fbHeight );

    Font::SetRenderBuffer( (UINT16 *)FrameBuffer::GetBackBuffer( ), fbWidth, fbHeight );

    char str[256];

    for (auto iterator = m_timers.Iterate(); iterator; ++iterator)
    {
        Value& v = iterator.Val();

        snprintf( str, sizeof( str ), "%s %dms", iterator.Key(), v.ns / 1000 );
        Font::Print( str, x + v.depth * MET_FONT_LETTER_WIDTH, y, 0xFFFF );

        y += fontHeight;
    }
}

void ShockTimerDisplay::Capture(scTreeNode<scTimer *> *node)
{
    static const float k = .1f;

    scTimer* timer = node->val;

    Value& v = m_timers[timer->Name()];
    v.depth = node->Depth();
    v.ns = v.ns * (1 - k) + (timer->Time() * k);
}
