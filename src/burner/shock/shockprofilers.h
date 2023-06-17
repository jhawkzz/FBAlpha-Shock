#ifndef SHOCKPROFILER_H_
#define SHOCKPROFILER_H_

#include "shock/shock.h"
#include "shock/core/ostimer.h"
#include "shock/util/hash_table.h"
#include "shock/util/tree.h"

static const UINT32 ShockProfilerCount = 64;

class ShockProfiler
{
public:
    ShockProfiler( )
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

    friend class ShockProfilers;
};

class ShockProfilers
{
private:
    typedef TreeNode<ShockProfiler*> ShockProfilerNode;

public:
    static void BeginScope(ShockProfiler* timer)
    {
        ShockProfilerNode* node;

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

    static void TraverseDepth(void* context, Tree<ShockProfiler*, ShockProfilerCount>::TreeCb cb)
    {
        mTree.TraverseDepth(context, cb);
    }

    static void TraverseBreadth(void* context, Tree<ShockProfiler*, ShockProfilerCount>::TreeCb cb)
    {
        mTree.TraverseBreadth(context, cb);
    }

    static ShockProfiler* GetShockProfiler(const char* name) { ShockProfiler* t = &mProfilers[name]; t->name = name; return t; }

    static void Clear() { mTree.Clear(); }

private:
    static HashTable<const char*, ShockProfiler, ShockProfilerCount> mProfilers;
    static Tree<ShockProfiler*, ShockProfilerCount> mTree;
    static ShockProfilerNode* mNode;
};

class ShockProfilerScope
{
public:
    ShockProfilerScope(const char* scope)
    {
        mShockProfiler = ShockProfilers::GetShockProfiler(scope);
        ShockProfilers::BeginScope(mShockProfiler);
    }

    ~ShockProfilerScope()
    {
        ShockProfilers::EndScope();
    }

private:
    ShockProfiler* mShockProfiler;
};

#ifdef SHOCK_PROFILERS
    #define SHOCK_PROFILE_SCOPE(scope) \
        ShockProfilerScope scope(#scope)

    #define SHOCK_PROFILE \
        ShockProfilerScope scope(__FUNCTION__)

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

#endif // SHOCKPROFILER_H_
