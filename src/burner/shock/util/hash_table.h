#ifndef SCHASH_TABLE_H_
#define SCHASH_TABLE_H_

#include "shock/shock.h"
#include "shock/util/array.h"
#include "shock/util/util.h"

inline UINT32 scHash(const char*);

template <class T, class K>
struct scAssocEntry
{
   K key;
   T val;
};

template <class T, class K, UINT32 C>
class scHashTable
{
private:
   typedef scAssocEntry<T,K> Assoc;
   
   struct Entry
   {
       Entry( )
       {
           next = NULL;
           hash = 0;
       }

       Assoc assoc;
       Entry* next;
       UINT32 hash;
   };

public:
   scHashTable()
       : m_entries(m_buffer, 0, C)
   {}

   Entry *AddEntry( K key, UINT32 hash );
   
   // scSpan(T* data, UINT32 size, UINT32 capacity)
   // : m_data(data)
   // , m_size(size)
   // , m_capacity(capacity)
   // {}
   
   // bool Append(T t); // returns true if appended, false if it was at capacity
   // UINT32 Append(const scSpan<const T>&); // appends up to Capacity(), returns actual amount appended

   // UINT32 Fill(UINT32 start, T data, UINT32 size); // fills single value up to Capacity(), returns actual amount filled
   // UINT32 Write(UINT32 start, const scSpan<const T> &); // writes up to Capacity(), returns actual amount written
   // UINT32 Read(UINT32 start, scSpan<T> &); // reads up to Capacity(), return s actual amount read

   // void Size(UINT32 size);

   // UINT32 Size() const { return m_size; }
   // UINT32 Capacity() const { return m_capacity; }
   // T* Data() const { return m_data; }
   // T* Ptr(UINT32 i) const { return &m_data[i]; }

   T& operator[](K key);

private:
   UINT32 GetBucket(UINT32 hash) const;

private:
    Entry m_buffer[C];
    
    scArray<Entry*, C / 16 + 1> m_buckets;
    scSpan<Entry> m_entries;

};

#include "hash_table.inl"

#endif // SCHASH_TABLE_H_