#ifndef ARRAY_H_
#define ARRAY_H_

#include "shock/shock.h"
#include "shock/util/span.h"

template <class T, UINT32 C>
class Array : public Span<T>
{
public:
   Array()
   : Span<T>(mbuffer, 0, C)
   {}   

public:
   operator Span<const UINT8>() { return Span<const UINT8>((const UINT8*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }
   operator Span<const char>() { return Span<const char>((const char*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }

   using Span<T>::Data;
   using Span<T>::Size;
   using Span<T>::Capacity;
   using Span<T>::Read;
   using Span<T>::Write;
   using Span<T>::Grow;
    
private:
    T mbuffer[C];
};

#endif // ARRAY_H_