#ifndef SCARRAY_H_
#define SCARRAY_H_

#include "shock/shock.h"
#include "shock/util/span.h"

template <class T, UINT32 C>
class scArray : public scSpan<T>
{
public:
   scArray()
   : scSpan<T>(m_buffer, C, C)
   {}   

public:
   operator scSpan<const byte>() { return scSpan<const byte>((const byte*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }
   operator scSpan<const char>() { return scSpan<const char>((const char*)Data(), Size() * sizeof(T), Capacity() * sizeof(T)); }

   using scSpan<T>::Data;
   using scSpan<T>::Size;
   using scSpan<T>::Capacity;
   using scSpan<T>::Read;
   using scSpan<T>::Write;

private:
    using scSpan<T>::Grow; // arrays can't grow
    
    T m_buffer[C];
};

#endif // SCARRAY_H_