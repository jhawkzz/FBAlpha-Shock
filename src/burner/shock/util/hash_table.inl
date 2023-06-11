
template <class K, class T, UINT32 C>
scHashTableIterator<K, T, C>& scHashTableIterator<K, T, C>::operator++()
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
scHashTableIterator<K, T, C>::operator bool()
{
    return !!entry;
}

template <class K, class T, UINT32 C>
scHashTableIterator<K, T, C>::scHashTableIterator(scHashTable<K, T, C>& hashTable)
    : table(hashTable)
{
    bucket = 0;
    entry = NULL;

    FindNext(bucket);
}

template <class K, class T, UINT32 C>
void scHashTableIterator<K, T, C>::FindNext(UINT32 start)
{
    UINT32 i;
    for (i = start; i < table.m_buckets.Size(); i++)
    {
        auto* e = table.m_buckets[i];
        if (!e) continue;

        bucket = i;
        entry = e;
        break;
    }
}

template <class K, class T, UINT32 C>
typename scHashTable<K, T, C>::Entry *scHashTable<K, T, C>::AddEntry( K key, UINT32 hash )
{
    Entry *entry = m_entries.Grow( );
    entry->hash = hash;
    entry->assoc.key = key;
    entry->assoc.val = T();

    return entry;
};

template <class K, class T, UINT32 C>
T& scHashTable<K, T, C>::operator[](K key)
{
    UINT32 hash = scHash(key);
    UINT32 bucket = GetBucket(hash);

    Entry*& start = m_buckets[bucket];

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
scHashTableIterator<K, T, C> scHashTable<K, T, C>::Iterator()
{
    return scHashTableIterator<K, T, C>(*this);
}

template <class K, class T, UINT32 C>
UINT32 scHashTable<K, T, C>::GetBucket(UINT32 hash) const
{
    return hash % m_buckets.Capacity();
}

inline UINT32 scHash(const char* val)
{
    return scHash((void*) val);
}

inline UINT32 scHash(const void* val)
{
    return (UINT32)((NUINT)val & 0xffffffff);
}
