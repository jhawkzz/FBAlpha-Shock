
template <class T>
bool scSpan<T>::Append(T t)
{
   return Write(Size(), scSpan<T>(&t, 1, 1)) ? true : false;
}

template <class T>
uint scSpan<T>::Append(const scSpan<const T>& t)
{
   return Write(Size(), t);
}

template <class T>
uint scSpan<T>::Fill(uint start, T data, uint size)
{
   if (start > Capacity())
      return 0;

   uint remain = Capacity() - start;
   size = remain <= size ? remain : size;

   T* d = Data() + start;

   for (uint i = 0; i < size; i++)
      d[i] = data;

   Size(Size() + size);
   return size;
}

template <class T>
uint scSpan<T>::Write(uint start, const scSpan<const T> &t)
{
   if (start > Capacity())
      return 0;

   uint remain = Capacity() - start;
   uint size = remain <= t.Size() ? remain : t.Size();

   scMemCopy(Data() + start, t.Data(), size * sizeof(T));

   uint modified = start + size;
   Size(modified < Size() ? Size() : modified);

   return size;
}

template <class T>
uint scSpan<T>::Read(uint start, scSpan<T> &t)
{
   if (start > Size())
      return 0;

   T* dest = t.Data();

   uint remain = Size() - start;
   uint size = remain <= t.Capacity() ? remain : t.Capacity();

   scMemCopy(dest, Data() + start, size * sizeof(T));
   
   t.Size(size);
   return size;
}

template <class T>
void scSpan<T>::Size(uint size) 
{ 
   ASSERT(size <= Capacity());
   m_size = size; 
}
