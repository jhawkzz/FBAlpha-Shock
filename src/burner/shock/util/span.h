#ifndef SPAN_H_
#define SPAN_H_

#include "shock/shock.h"
#include "shock/util/util.h"

template <class T>
class Span
{
public:
   Span(T* data, UINT32 size, UINT32 capacity)
   : mData(data)
   , mSize(size)
   , mCapacity(capacity)
   {}
   
   bool Append(T t); // returns true if appended, false if it was at capacity
   UINT32 Append(const Span<const T>&); // appends up to Capacity(), returns actual amount appended

   UINT32 Fill(UINT32 start, T data, UINT32 size); // fills single value up to Capacity(), returns actual amount filled
   UINT32 Write(UINT32 start, const Span<const T> &); // writes up to Capacity(), returns actual amount written
   UINT32 Read(UINT32 start, Span<T> &); // reads up to Capacity(), return s actual amount read

   void Size(UINT32 size);
   T& Grow() { UINT32 s = Size(); Size(s + 1); return mData[s]; }

   UINT32 Size() const { return mSize; }
   UINT32 Capacity() const { return mCapacity; }
   T* Data() const { return mData; }

   void Clear() { mSize = 0; }

   T& operator[](UINT32 i) const { SHOCK_ASSERT(i < Size()); return mData[i]; }

   operator Span<const T>() { return Span<const T>((const T*)Data(), Size(), Capacity()); }

public:
   T* mData;
   UINT32 mSize;
   UINT32 mCapacity;
};

#include "span.inl"

#endif // SPAN_H_