#ifndef SCHASH_TABLE_H_
#define SCHASH_TABLE_H_

#include "shock/shock.h"
#include "shock/util/array.h"
#include "shock/util/hash.h"
#include "shock/util/util.h"

template <class K, class T>
struct scAssocEntry
{
    K key;
    T val;
};

template <class K, class T>
struct scHashTableEntry
{
    scHashTableEntry()
    {
        next = NULL;
        hash = 0;
    }

    scAssocEntry<K, T> assoc;
    scHashTableEntry<K, T>* next;
    NUINT hash;
};

template <class K, class T, UINT32 C>
class scHashTable;

template <class K, class T, UINT32 C>
struct scHashTableIterator
{
public:
    K& Key() { return entry->assoc.key; }
    T& Val() { return entry->assoc.val; }

    scHashTableIterator<K, T, C>& operator++();
    operator bool();

private:
    scHashTableIterator(scHashTable<K, T, C>& hashTable);
    void FindNext(UINT32 start);

private:
    scHashTable<K, T, C>& table;
    scHashTableEntry<K, T> *entry;
    UINT32 bucket;

    template <class K, class T, UINT32 C>
    friend class scHashTable;
};

template <class K, class T, UINT32 C>
class scHashTable
{
private:
    typedef scAssocEntry<K, T> Assoc;
    typedef scHashTableEntry<K, T> Entry;

public:
    scHashTable()
        : m_entries(m_buffer, 0, C)
    {
        m_buckets.Fill(0, 0, m_buckets.Capacity());
    }

    Entry* AddEntry(K key, NUINT hash);

    T& operator[](K key);

    scHashTableIterator<K, T, C> Iterator();

private:
    UINT32 GetBucket(NUINT hash) const;

private:
    Entry m_buffer[C];

    scArray<Entry*, C / 16 + 1> m_buckets;
    scSpan<Entry> m_entries;

    template <class K, class T, UINT32 C>
    friend struct scHashTableIterator;
};

#include "hash_table.inl"

#endif // SCHASH_TABLE_H_