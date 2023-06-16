#ifndef TIMER_H_
#define TIMER_H_

#include "shock/shock.h"
#include "shock/core/ostimer.h"
#include "shock/util/hash_table.h"
#include "shock/util/tree.h"

static const UINT32 TimerCount = 256;

class Timer
{
public:
    Timer( )
    {
        time = 0;
        name = NULL;
    }

    void Start()
    {
        timer.Reset();
    }

    void Stop()
    {
        time = timer.GetElapsedTimeMicroseconds();
    }

    UINT32 Time() const { return time; }
    const char* Name() const { return name; }

private:
    OSTimer timer;
    UINT32 time;
    const char* name;

    friend class TimerTree;
};

class TimerTree
{
private:
    typedef TreeNode<Timer*> TimerNode;

public:
    static void BeginScope(Timer* timer)
    {
        TimerNode* node;

        if (!m_node)
            node = m_tree.Head();
        else
            node = m_tree.AddChild(m_node);

        node->val = timer;
        m_node = node;
        m_node->val->Start();
    }

    static void EndScope()
    {
        m_node->val->Stop();
        m_node = m_node->parent;
    }

    static void TraverseDepth(void* context, Tree<Timer*, 64>::TreeCb cb)
    {
        m_tree.TraverseDepth(context, cb);
    }

    static void TraverseBreadth(void* context, Tree<Timer*, 64>::TreeCb cb)
    {
        m_tree.TraverseBreadth(context, cb);
    }

    static Timer* GetTimer(const char* name) { Timer* t = &m_timers[name]; t->name = name; return t; }

    static void Clear() { m_tree.Clear(); }

private:
    static HashTable<const char*, Timer, TimerCount> m_timers;
    static Tree<Timer*, TimerCount> m_tree;
    static TimerNode* m_node;
};

class TimerScope
{
public:
    TimerScope(const char* scope)
    {
        m_timer = TimerTree::GetTimer(scope);
        TimerTree::BeginScope(m_timer);
    }

    ~TimerScope()
    {
        TimerTree::EndScope();
    }

private:
    Timer* m_timer;
};

#define SHOCK_PROFILE_SCOPE(scope) \
    TimerScope scope(#scope)

#define SHOCK_PROFILE \
    TimerScope scope(__FUNCTION__)

#define BURN_SCOPE SHOCK_PROFLE

#ifndef BURN_SCOPE
#define BURN_SCOPE(scope) 0
#endif

#endif // TIMER_H_
