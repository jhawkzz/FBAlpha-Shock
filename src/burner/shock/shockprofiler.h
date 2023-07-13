#ifndef SHOCKPROFILER_H_
#define SHOCKPROFILER_H_

#include "shock/shock.h"
#include "shock/core/ostimer.h"
#include "shock/util/hash_table.h"
#include "shock/util/tree.h"

#ifdef SHOCK_PROFILER

static const UINT32 ShockProfilerNodeCount = 64;

class ShockProfilerNode
{
public:
    ShockProfilerNode( )
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
        time = (UINT32) timer.GetElapsedTimeMicroseconds();
    }

    UINT32 Time() const { return time; }
    const char* Name() const { return name; }

private:
    OSTimer timer;
    UINT32 time;
    const char* name;

    friend class ShockProfiler;
};

class ShockProfiler
{
private:
    typedef TreeNode<ShockProfilerNode*> ShockProfilerTreeNode;

public:
    static void BeginScope(ShockProfilerNode* node)
    {
        ShockProfilerTreeNode* treeNode;

        if (!mTreeNode)
            treeNode = mTree.Head();
        else
            treeNode = mTree.AddChild(mTreeNode);

        treeNode->val = node;
        mTreeNode = treeNode;
        mTreeNode->val->Start();
    }

    static void EndScope()
    {
        mTreeNode->val->Stop();
        mTreeNode = mTreeNode->parent;
    }

    static void TraverseDepth(void* context, Tree<ShockProfilerNode*, ShockProfilerNodeCount>::TreeCb cb)
    {
        mTree.TraverseDepth(context, cb);
    }

    static void TraverseBreadth(void* context, Tree<ShockProfilerNode*, ShockProfilerNodeCount>::TreeCb cb)
    {
        mTree.TraverseBreadth(context, cb);
    }

    static ShockProfilerNode* GetShockProfilerNode(const char* name) { ShockProfilerNode* t = &mNodes[name]; t->name = name; return t; }

    static void Clear() { mTree.Clear(); }

private:
    static HashTable<const char*, ShockProfilerNode, ShockProfilerNodeCount> mNodes;
    static Tree<ShockProfilerNode*, ShockProfilerNodeCount> mTree;
    static ShockProfilerTreeNode* mTreeNode;
};

class ShockProfilerScope
{
public:
    ShockProfilerScope(const char* scope)
    {
        mNode = ShockProfiler::GetShockProfilerNode(scope);
        ShockProfiler::BeginScope(mNode);
    }

    ~ShockProfilerScope()
    {
        ShockProfiler::EndScope();
    }

private:
    ShockProfilerNode* mNode;
};

#define SHOCK_PROFILE_SCOPE(scope) \
    ShockProfilerScope scope(#scope)

#define SHOCK_PROFILE \
    ShockProfilerScope scope(__FUNCTION__)

#define BURN_PROFILE_SCOPE(s) SHOCK_PROFLE_SCOPE(s)
    #define BURN_PROFILE SHOCK_PROFLE
#else // SHOCK_PROFILER
    #define SHOCK_PROFILE_SCOPE(scope)
    #define SHOCK_PROFILE
    #define BURN_PROFILE_SCOPE(s)
    #define BURN_PROFILE SHOCK_PROFLE
#endif

#ifndef BURN_SCOPE
    #define BURN_SCOPE(scope) 0
#endif

#endif // SHOCKPROFILER_H_
