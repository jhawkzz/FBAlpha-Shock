
template <class T, UINT32 C>
T& Stack<T, C>::Top()
{
    return mArray[mArray.Size() - 1];
}

template <class T, UINT32 C>
void Stack<T, C>::Pop()
{
    mArray.Size(mArray.Size() - 1);
}

template <class T, UINT32 C>
void Stack<T, C>::Push(T t)
{
     mArray.Grow() = t;
}
