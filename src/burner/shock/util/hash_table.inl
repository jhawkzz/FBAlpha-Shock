template <class T, class K, UINT32 C>
T& scHashTable<T, K, C>::operator[](K key)
{
    UINT32 hash = scHash(key);
    UINT32 bucket = GetBucket(hash);

    Entry*& entry = m_buckets[bucket];

    while (1)
    {
        if (!entry)
        {
            entry = m_entries.Grow();
            entry->hash = hash;
            entry->assoc.key = key;
        }

        if (entry->hash == hash)
            return entry->assoc.val;

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
