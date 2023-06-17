
template <class T>
bool Span<T>::Append(T t)
{
   return Write(Size(), Span<T>(&t, 1, 1)) ? true : false;
}

template <class T>
UINT32 Span<T>::Append(const Span<const T>& t)
{
   return Write(Size(), t);
}

template <class T>
UINT32 Span<T>::Fill(UINT32 start, T data, UINT32 size)
{
   if (start > Capacity())
      return 0;

   UINT32 remain = Capacity() - start;
   size = remain <= size ? remain : size;

   T* d = Data() + start;

   for (UINT32 i = 0; i < size; i++)
      d[i] = data;

   Size(Size() + size);
   return size;
}

template <class T>
UINT32 Span<T>::Write(UINT32 start, const Span<const T> &t)
{
   if (start > Capacity())
      return 0;

   UINT32 remain = Capacity() - start;
   UINT32 size = remain <= t.Size() ? remain : t.Size();

   memcpy(Data() + start, t.Data(), size * sizeof(T));

   UINT32 modified = start + size;
   Size(modified < Size() ? Size() : modified);

   return size;
}

template <class T>
UINT32 Span<T>::Read(UINT32 start, Span<T> &t)
{
   if (start > Size())
      return 0;

   T* dest = t.Data();

   UINT32 remain = Size() - start;
   UINT32 size = remain <= t.Capacity() ? remain : t.Capacity();

   memcpy(dest, Data() + start, size * sizeof(T));
   
   t.Size(size);
   return size;
}

template <class T>
void Span<T>::Size(UINT32 size) 
{ 
   SC_ASSERT(size <= Capacity());
   mSize = size; 
}
