#ifndef SCTIMER_H_
#define SCTIMER_H_

#include "shock/shock.h"
#include "shock/core/ostimer.h"
#include "shock/util/hash_table.h"
#include "shock/util/tree.h"

class scTimer
{
public:
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
    UINT32 time = 0;
    const char* name = nullptr;

    friend class scTimerTree;
};

class scTimerTree
{
private:
    typedef scTreeNode<scTimer*> TimerNode;

public:
    static void BeginScope(scTimer* timer)
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

        scTimer* timer = m_node->val;
        m_node = m_node->Parent();
    }

    static void TraverseDepth(void* context, scTree<scTimer*, 64>::scTreeCb cb)
    {
        m_tree.TraverseDepth(context, cb);
    }

    static void TraverseBreadth(void* context, scTree<scTimer*, 64>::scTreeCb cb)
    {
        m_tree.TraverseBreadth(context, cb);
    }

    static scTimer* GetTimer(const char* name) { scTimer* t = &m_timers[name]; t->name = name; return t; }

    static void Clear() { m_tree.Clear(); }

private:
    static scHashTable<scTimer, const char*, 64> m_timers;
    static scTree<scTimer*, 64> m_tree;
    static TimerNode* m_node;
};

class scTimerScope
{
public:
    scTimerScope(const char* scope)
    {
        m_timer = scTimerTree::GetTimer(scope);
        scTimerTree::BeginScope(m_timer);
    }

    ~scTimerScope()
    {
        scTimerTree::EndScope();
    }

private:
    scTimer* m_timer;
};

#define SHOCK_PROFILE_SCOPE(scope) \
    scTimerScope scope(#scope)

#define SHOCK_PROFILE \
    scTimerScope scope(##__FUNCTION__)

#define BURN_SCOPE SHOCK_PROFLE

#ifndef BURN_SCOPE
#define BURN_SCOPE(scope) 0
#endif

#endif // SCTIMER_H_
