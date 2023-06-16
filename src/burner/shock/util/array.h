#ifndef ARRAY_H_
#define ARRAY_H_

#include "shock/shock.h"
#include "shock/util/span.h"

template <class T, UINT32 C>
class Array : public Span<T>
{
public:
   Array()
   : Span<T>(m_buffer, 0, C)
   {}   

public:
   operator Span<const byte>() { return Span<const byte>((const byte*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }
   operator Span<const char>() { return Span<const char>((const char*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }

   using Span<T>::Data;
   using Span<T>::Size;
   using Span<T>::Capacity;
   using Span<T>::Read;
   using Span<T>::Write;

private:
    using Span<T>::Grow; // arrays can't grow
    
    T m_buffer[C];
};

#endif // ARRAY_H_