#ifndef TREE_H_
#define TREE_H_

#include "shock/shock.h"
#include "shock/util/array.h"

template <class T>
struct TreeNode
{
public:
    TreeNode( )
    {
        firstChild = NULL;
        prevSibling = NULL;
        nextSibling = NULL;
        parent = NULL;
        depth = 0;
    }

    TreeNode<T>* AddChild(TreeNode<T>* child)
    {
        child->depth = depth + 1;
        child->parent = this;
        child->nextSibling = firstChild;
        child->prevSibling = NULL;
        child->firstChild = NULL;

        if (firstChild)
            firstChild->prevSibling = child;

        firstChild = child;
        return child;
    }

public:
    T val;

public:
    TreeNode* firstChild;
    TreeNode* prevSibling;
    TreeNode* nextSibling;
    TreeNode* parent;
    UINT32 depth;
};

template <class T, UINT32 C>
class Tree
{
public:
    typedef TreeNode<T> Node;
    typedef bool(*TreeCb)(void* context, TreeNode<T>*);

public:
    Tree()
        : head(NULL)
    {}

    Node* Head() { head = (!memory.Size() ? memory.Grow() : &memory[0]); return head; }

    Node* AddChild(Node* node)
    {
        return node->AddChild(Alloc());
    }

    void TraverseDepth(void* context, TreeCb cb)
    {
        TraverseDepth(head, context, cb);
    }

    void TraverseBreadth(void* context, TreeCb cb)
    {
        TraverseBreadth(head, context, cb);
    }

    void Clear()
    {
        memory[0] = Node();
        memory.Size(1);

        head = NULL;
    }

    Node* Alloc()
    {
        if (memory.Size() == memory.Capacity())
            return NULL;

        return memory.Grow();
    }

private:
    bool TraverseDepth(Node* node, void* context, TreeCb cb)
    {
        if (!node)
            return true;

        if (!cb(context, node))
            return false;

        if (!TraverseDepth(node->firstChild, context, cb))
            return false;

        return TraverseDepth(node->nextSibling, context, cb);
    }

    bool TraverseBreadth(Node* node, void* context, TreeCb cb)
    {
        if (!node)
            return true;

        if (!cb(context, node))
            return false;

        if (!TraverseBreadth(node->nextSibling, context, cb))
            return false;
        
        return TraverseBreadth(node->firstChild, context, cb);
    }

private:
    Node* head;
    Array<Node, C> memory;
};

#endif // TREE_H_