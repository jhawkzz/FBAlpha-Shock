#ifndef SCTREE_H_
#define SCTREE_H_

template <class T>
struct scTreeNode
{
public:
   scTreeNode<T>* AddChild(scTreeNode<T>* child)
   {
      child->parent = this;
      child->sibling = firstChild;
      child->firstChild = nullptr;
      firstChild = child;

      return child;
   }

   scTreeNode* FirstChild() { return firstChild; }
   scTreeNode* Sibling() { return sibling; }
   scTreeNode* Parent() { return parent; }

public:
   T val;

private:
   scTreeNode* firstChild = nullptr;
   scTreeNode* sibling = nullptr;
   scTreeNode* parent = nullptr;
};

template <class T, uint S>
class scTree
{
public:
   typedef scTreeNode<T> Node;
   typedef void(*scTreeCb)(void* context, scTreeNode<T>*);

public:
   scTree() { memory.Append(scTreeNode()) }

   Node* Head() { return memory.Data(0); }

   Node* AddChild(Node* node)
   {
      return node->AddChild(Alloc());
   }

   void TraverseDepth(void* context, scTreeCb cb)
   {
      TraverseDepth(Head(), context, cb);
   }

   void TraverseBreadth(void* context, scTreeCb cb)
   {
      TraverseBreadth(Head(), context, cb);
   }  

private:
   Node* Alloc() 
   { 
      if (memory.Size() == memory.Capacity())
         return nullptr;

      uint slot = memory.Size() + 1;
      memory.Size(slot);
      return memory.Data(slot);
   }

   void TraverseDepth(Node* node, void* context, scTreeCb cb)
   {
      if (!node)
         return;

      cb(context, node);
       
      TraverseDepth(node->FirstChild(), context, cb);
      TraverseDepth(node->Sibling(), context,cb);
   }

   void TraverseBreadth(Node* node, void* context, scTreeCb cb)
   {
      if (!node)
         return;

      cb(context, node);

      TraverseBreadth(node->Sibling(), context, cb);
      TraverseBreadth(node->FirstChild(), context, cb);
   }

private:
   scArray<Node, S> memory;
};

#endif // SCTREE_H_