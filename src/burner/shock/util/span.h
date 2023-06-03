#ifndef SCSPAN_H_
#define SCSPAN_H_

#include "shock/shock.h"
#include "shock/util/util.h"

template <class T>
class scSpan
{
public:
   scSpan(T* data, UINT32 size, UINT32 capacity)
   : m_data(data)
   , m_size(size)
   , m_capacity(capacity)
   {}
   
   bool Append(T t); // returns true if appended, false if it was at capacity
   UINT32 Append(const scSpan<const T>&); // appends up to Capacity(), returns actual amount appended

   UINT32 Fill(UINT32 start, T data, UINT32 size); // fills single value up to Capacity(), returns actual amount filled
   UINT32 Write(UINT32 start, const scSpan<const T> &); // writes up to Capacity(), returns actual amount written
   UINT32 Read(UINT32 start, scSpan<T> &); // reads up to Capacity(), return s actual amount read

   void Size(UINT32 size);
   T* Grow() { UINT32 s = Size(); Size(s + 1); return &m_data[s]; }

   UINT32 Size() const { return m_size; }
   UINT32 Capacity() const { return m_capacity; }
   T* Data() const { return m_data; }

   T& operator[](UINT32 i) const { SC_ASSERT(i < Size()); return m_data[i]; }

   operator scSpan<const T>() { return scSpan<const T>((const T*)Data(), Size(), Capacity()); }

public:
   T* m_data;
   UINT32 m_size;
   UINT32 m_capacity;
};

#include "span.inl"

#endif // SCSPAN_H_