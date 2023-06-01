#ifndef SCSPAN_H_
#define SCSPAN_H_

#include "tomahawk.h"
#include "tomahawk/core/assert.h"
#include "tomahawk/core/memory.h"

template <class T>
class scSpan
{
public:
   scSpan(T* data, uint size, uint capacity)
   : m_data(data)
   , m_size(size)
   , m_capacity(capacity)
   {}
   
   bool Append(T t); // returns true if appended, false if it was at capacity
   uint Append(const scSpan<const T>&); // appends up to Capacity(), returns actual amount appended

   uint Fill(uint start, T data, uint size); // fills single value up to Capacity(), returns actual amount filled
   uint Write(uint start, const scSpan<const T> &); // writes up to Capacity(), returns actual amount written
   uint Read(uint start, scSpan<T> &); // reads up to Capacity(), return s actual amount read

   void Size(uint size);

   uint Size() const { return m_size; }
   uint Capacity() const { return m_capacity; }
   T* Data() const { return m_data; }

   T operator[](uint i) const { ASSERT(i < Size()); return m_data[i]; }

   operator scSpan<const T>() { return scSpan<const T>((const T*)Data(), Size(), Capacity()); }

public:
   T* m_data;
   uint m_size;
   uint m_capacity;
};

#include "span.inl"

#endif // SCSPAN_H_