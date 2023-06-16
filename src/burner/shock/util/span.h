#ifndef SPAN_H_
#define SPAN_H_

#include "shock/shock.h"
#include "shock/util/util.h"

template <class T>
class Span
{
public:
   Span(T* data, UINT32 size, UINT32 capacity)
   : m_data(data)
   , m_size(size)
   , m_capacity(capacity)
   {}
   
   bool Append(T t); // returns true if appended, false if it was at capacity
   UINT32 Append(const Span<const T>&); // appends up to Capacity(), returns actual amount appended

   UINT32 Fill(UINT32 start, T data, UINT32 size); // fills single value up to Capacity(), returns actual amount filled
   UINT32 Write(UINT32 start, const Span<const T> &); // writes up to Capacity(), returns actual amount written
   UINT32 Read(UINT32 start, Span<T> &); // reads up to Capacity(), return s actual amount read

   void Size(UINT32 size);
   T* Grow() { UINT32 s = Size(); Size(s + 1); return &m_data[s]; }

   UINT32 Size() const { return m_size; }
   UINT32 Capacity() const { return m_capacity; }
   T* Data() const { return m_data; }

   void Clear() { m_size = 0; }

   T& operator[](UINT32 i) const { SC_ASSERT(i < Size()); return m_data[i]; }

   operator Span<const T>() { return Span<const T>((const T*)Data(), Size(), Capacity()); }

public:
   T* m_data;
   UINT32 m_size;
   UINT32 m_capacity;
};

#include "span.inl"

#endif // SPAN_H_