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
        sibling = NULL;
        parent = NULL;
        depth = 0;
    }

    TreeNode<T>* AddChild(TreeNode<T>* child)
    {
        child->depth = depth + 1;
        child->parent = this;
        child->sibling = firstChild;
        child->firstChild = NULL;
        firstChild = child;

        return child;
    }

public:
    T val;

public:
    TreeNode* firstChild;
    TreeNode* sibling;
    TreeNode* parent;
    UINT32 depth;
};

template <class T, UINT32 C>
class Tree
{
public:
    typedef TreeNode<T> Node;
    typedef void(*TreeCb)(void* context, TreeNode<T>*);

public:
    Tree()
        : memory(buffer, 1, C)
        , head(NULL)
    {}

    Node* Head() { head = &memory[0]; return head; }

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

        head = nullptr;
    }

    Node* Alloc()
    {
        if (memory.Size() == memory.Capacity())
            return NULL;

        return memory.Grow();
    }

private:
    void TraverseDepth(Node* node, void* context, TreeCb cb)
    {
        if (!node)
            return;

        cb(context, node);

        TraverseDepth(node->firstChild, context, cb);
        TraverseDepth(node->sibling, context, cb);
    }

    void TraverseBreadth(Node* node, void* context, TreeCb cb)
    {
        if (!node)
            return;

        cb(context, node);

        TraverseBreadth(node->sibling, context, cb);
        TraverseBreadth(node->firstChild, context, cb);
    }

private:
    Node* head;
    Node buffer[C];
    Span<Node> memory;
};

#endif // TREE_H_