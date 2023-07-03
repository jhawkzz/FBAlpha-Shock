#ifndef STACK_H_
#define STACK_H_

#include "shock/shock.h"
#include "shock/util/array.h"

template <class T, UINT32 C>
class Stack
{
public:
   Stack()
   {}   

public:
    T& Top();

    void Pop();
    void Push(T t);
    void Size() { return mArray.Size(); }

private:
    Array<T, C> mArray;
};

#include "stack.inl"

#endif // STACK_H_

