template <class T, class K, UINT32 C>
typename scHashTable<T, K, C>::Entry *scHashTable<T, K, C>::AddEntry( K key, UINT32 hash )
{
    Entry *entry = m_entries.Grow( );
    entry->hash = hash;
    entry->assoc.key = key;
    //entry->assoc.val = NULL;

    return entry;
};

template <class T, class K, UINT32 C>
T& scHashTable<T, K, C>::operator[](K key)
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

template <class T, class K, UINT32 C>
UINT32 scHashTable<T, K, C>::GetBucket(UINT32 hash) const
{
    return hash % m_buckets.Capacity();
}

inline UINT32 scHash(const char* val)
{
    return (UINT32)((NUINT)val & 0xffffffff);
}
