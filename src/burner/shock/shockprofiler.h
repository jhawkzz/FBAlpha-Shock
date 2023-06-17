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

        if (!mNode)
            node = mTree.Head();
        else
            node = mTree.AddChild(mNode);

        node->val = timer;
        mNode = node;
        mNode->val->Start();
    }

    static void EndScope()
    {
        mNode->val->Stop();
        mNode = mNode->parent;
    }

    static void TraverseDepth(void* context, Tree<Timer*, 64>::TreeCb cb)
    {
        mTree.TraverseDepth(context, cb);
    }

    static void TraverseBreadth(void* context, Tree<Timer*, 64>::TreeCb cb)
    {
        mTree.TraverseBreadth(context, cb);
    }

    static Timer* GetTimer(const char* name) { Timer* t = &mTimers[name]; t->name = name; return t; }

    static void Clear() { mTree.Clear(); }

private:
    static HashTable<const char*, Timer, TimerCount> mTimers;
    static Tree<Timer*, TimerCount> mTree;
    static TimerNode* mNode;
};

class TimerScope
{
public:
    TimerScope(const char* scope)
    {
        mTimer = TimerTree::GetTimer(scope);
        TimerTree::BeginScope(mTimer);
    }

    ~TimerScope()
    {
        TimerTree::EndScope();
    }

private:
    Timer* mTimer;
};

#ifdef SHOCK_TIMERS
    #define SHOCK_PROFILE_SCOPE(scope) \
        TimerScope scope(#scope)

    #define SHOCK_PROFILE \
        TimerScope scope(__FUNCTION__)

    #define BURN_PROFILE_SCOPE(s) SHOCK_PROFLE_SCOPE(s)
    #define BURN_PROFILE SHOCK_PROFLE
#else
    #define SHOCK_PROFILE_SCOPE(scope)
    #define SHOCK_PROFILE
    #define BURN_PROFILE_SCOPE(s)
    #define BURN_PROFILE SHOCK_PROFLE
#endif

#ifndef BURN_SCOPE
    #define BURN_SCOPE(scope) 0
#endif

#endif // TIMER_H_
