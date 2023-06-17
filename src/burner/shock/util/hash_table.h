#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "shock/shock.h"
#include "shock/util/array.h"
#include "shock/util/hash.h"
#include "shock/util/util.h"

template <class K, class T>
struct AssocEntry
{
    K key;
    T val;
};

template <class K, class T>
struct HashTableEntry
{
    HashTableEntry()
    {
        next = NULL;
        hash = 0;
    }

    AssocEntry<K, T> assoc;
    HashTableEntry<K, T>* next;
    NUINT hash;
};

template <class K, class T, UINT32 C>
class HashTable;

template <class K, class T, UINT32 C>
struct HashTableIterator
{
public:
    K& Key() { return entry->assoc.key; }
    T& Val() { return entry->assoc.val; }

    HashTableIterator<K, T, C>& operator++();
    operator bool();

private:
    HashTableIterator(HashTable<K, T, C>& hashTable);
    void FindNext(UINT32 start);

private:
    HashTable<K, T, C>& table;
    HashTableEntry<K, T> *entry;
    UINT32 bucket;

    template <class K, class T, UINT32 C>
    friend class HashTable;
};

template <class K, class T, UINT32 C>
class HashTable
{
private:
    typedef AssocEntry<K, T> Assoc;
    typedef HashTableEntry<K, T> Entry;

public:
    HashTable()
    {
        mBuckets.Fill(0, 0, mBuckets.Capacity());
    }

    Entry* AddEntry(K key, NUINT hash);

    T& operator[](K key);

    HashTableIterator<K, T, C> Iterator();

private:
    UINT32 GetBucket(NUINT hash) const;

private:
    Array<Entry, C> mEntries;
    Array<Entry*, C / 16 + 1> mBuckets;

    template <class K, class T, UINT32 C>
    friend struct HashTableIterator;
};

#include "hash_table.inl"

#endif // HASH_TABLE_H_