template <class T, class K, UINT32 C>
T& scHashTable<T, K, C>::operator[](K key)
{
    auto addEntry = [&](K key, UINT32 hash) -> Entry*
    {
        Entry* entry = m_entries.Grow();
        entry->hash = hash;
        entry->assoc.key = key;

        return entry;
    };

    UINT32 hash = scHash(key);
    UINT32 bucket = GetBucket(hash);

    Entry*& start = m_buckets[bucket];

    if (!start)
        start = addEntry(key, hash);

    Entry* entry = start;

    while (1)
    {
        if (entry->hash == hash)
            return entry->assoc.val;

        if (!entry->next)
            entry->next = addEntry(key, hash);

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
