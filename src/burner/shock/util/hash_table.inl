
template <class K, class T, UINT32 C>
HashTableIterator<K, T, C>& HashTableIterator<K, T, C>::operator++()
{
    if (entry)
    {
        entry = entry->next;
    
        if (!entry)
           FindNext(bucket + 1);
    }

    return *this;
}

template <class K, class T, UINT32 C>
HashTableIterator<K, T, C>::operator bool()
{
    return !!entry;
}

template <class K, class T, UINT32 C>
HashTableIterator<K, T, C>::HashTableIterator(HashTable<K, T, C>& hashTable)
    : table(hashTable)
{
    bucket = 0;
    entry = NULL;

    FindNext(bucket);
}

template <class K, class T, UINT32 C>
void HashTableIterator<K, T, C>::FindNext(UINT32 start)
{
    UINT32 i;
    for (i = start; i < table.mBuckets.Size(); i++)
    {
        auto* e = table.mBuckets[i];
        if (!e) continue;

        bucket = i;
        entry = e;
        break;
    }
}

template <class K, class T, UINT32 C>
typename HashTable<K, T, C>::Entry *HashTable<K, T, C>::AddEntry( K key, NUINT hash )
{
    Entry *entry = mEntries.Grow( );
    entry->hash = hash;
    entry->assoc.key = key;
    entry->assoc.val = T();

    return entry;
};

template <class K, class T, UINT32 C>
T& HashTable<K, T, C>::operator[](K key)
{
    NUINT hash = Hash(key);
    UINT32 bucket = GetBucket(hash);

    Entry*& start = mBuckets[bucket];

    if (!start)
        start = AddEntry(key, hash);

    Entry* entry = start;

    while (1)
    {
        if (entry->hash == hash)
            return entry->assoc.val;

        if (!entry->next)
            entry->next = AddEntry(key, hash);

        entry = entry->next;
    }
}

template <class K, class T, UINT32 C>
HashTableIterator<K, T, C> HashTable<K, T, C>::Iterator()
{
    return HashTableIterator<K, T, C>(*this);
}

template <class K, class T, UINT32 C>
UINT32 HashTable<K, T, C>::GetBucket(NUINT hash) const
{
    return hash % mBuckets.Capacity();
}
