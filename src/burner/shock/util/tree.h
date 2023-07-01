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
    {
        Clear();
    }

    Node* Head() { return head; }

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
        memory.Size(1);
        memory[0] = Node();
        head = &memory[0];
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

        bool continueDepth = cb(context, node);

        if (continueDepth)
            continueDepth = TraverseDepth(node->firstChild, context, cb);

        TraverseDepth(node->nextSibling, context, cb);
        return continueDepth;
    }

    bool TraverseBreadth(Node* node, void* context, TreeCb cb)
    {
        if (!node)
            return true;

        bool continueBreadth = cb(context, node);

        if (continueBreadth)
            continueBreadth = TraverseBreadth(node->nextSibling, context, cb);
        
        TraverseBreadth(node->firstChild, context, cb);
        return continueBreadth;
    }

private:
    Node* head;
    Array<Node, C> memory;
};

#endif // TREE_H_