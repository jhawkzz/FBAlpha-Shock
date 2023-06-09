#ifndef SCTREE_H_
#define SCTREE_H_

#include "shock/shock.h"
#include "shock/util/array.h"

template <class T>
struct scTreeNode
{
public:
    scTreeNode<T>* AddChild(scTreeNode<T>* child)
    {
        child->depth = depth + 1;
        child->parent = this;
        child->sibling = firstChild;
        child->firstChild = nullptr;
        firstChild = child;

        return child;
    }

    scTreeNode* FirstChild() const { return firstChild; }
    scTreeNode* Sibling() const { return sibling; }
    scTreeNode* Parent() const { return parent; }
    UINT32 Depth() const { return depth; }

public:
    T val;

private:
    scTreeNode* firstChild = nullptr;
    scTreeNode* sibling = nullptr;
    scTreeNode* parent = nullptr;
    UINT32 depth = 0;
};

template <class T, UINT32 C>
class scTree
{
public:
    typedef scTreeNode<T> Node;
    typedef void(*scTreeCb)(void* context, scTreeNode<T>*);

public:
    scTree()
        : memory(m_buffer, 1, C)
    {}

    Node* Head() { return &memory[0]; }

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

    void Clear()
    {
        *Head() = Node();
        memory.Size(1);
    }

private:
    Node* Alloc()
    {
        if (memory.Size() == memory.Capacity())
            return nullptr;

        return memory.Grow();
    }

    void TraverseDepth(Node* node, void* context, scTreeCb cb)
    {
        if (!node)
            return;

        cb(context, node);

        TraverseDepth(node->FirstChild(), context, cb);
        TraverseDepth(node->Sibling(), context, cb);
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
    Node m_buffer[C];
    scSpan<Node> memory;
};

#endif // SCTREE_H_