#ifndef SHOCKTIMER_H_
#define SHOCKTIMER_H_

#include "shock/shock.h"

class ShockTimer
{
public:
   ShockTimer(const char* name)
      : m_name(name)
      //, m_time(0)
   {
   }

   void Start()
   {
      //OSTimer::Start();
   }

   void Stop()
   {
      //m_time = OSTimer::End();
   }

   NUINT Hash() const { return (NUINT) m_name; }

private:
   const char* m_name;
   //const time m_time;
};

class ShockTimerTree
{
private:
   typedef ShockTreeNode<ShockTimer*> TimerNode;

public:
   static void BeginScope(ShockTimer* timer)
   {
      TimerNode* node;

      if (!m_node)
         node = m_tree.Head();
      else
         node = m_tree.AddChild(m_node);

      node->val = timer;
      node->val->Start();
      m_node = node;
   }

   static void EndScope()
   {
      m_node->val->Stop(); 
      m_node = m_node->Parent();
   }

   static void TraverseDepth(void* context, ShockTree<ShockTimer*, 64>::ShockTreeCb cb)
   {
      m_tree.TraverseDepth(context, cb);
   }

   static void TraverseBreadth(void* context, ShockTree<ShockTimer*, 64>::ShockTreeCb cb)
   {
      m_tree.TraverseBreadth(context, cb);
   }

private:
   static ShockTree<ShockTimer*, 64> m_tree;
   static TimerNode* m_node;
};

class ShockTimerScope
{
public:
   ShockTimerScope(const char* scope)
      : m_timer(scope)
   {
      ShockTimerTree::BeginScope(&m_timer);
   }

   ~ShockTimerScope()
   {
      ShockTimerTree::EndScope();
   }

private:
   ShockTimer m_timer;
};

#define SHOCK_PROFILE_COPE(scope)\
ShockTimerScope scope(#scope)

#define SHOCK_PROFILE\
ShockTimerScope scope(#_FUNCTION_)

#define BURN_SCOPE SHOCK_PROFLE

#ifndef BURN_SCOPE
#define BURN_SCOPE(scope) 0
#endif

#endif // SHOCKTIMER_H_
