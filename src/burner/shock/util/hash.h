#ifndef HASH_H_
#define HASH_H_

#include "shock/shock.h"

const NUINT HashDefault = 0;

inline NUINT Hash(NUINT value, NUINT seed)
{
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

#ifdef SHOCK_64BIT
    const NUINT offset_basis = 14695981039346656037;
    const NUINT prime = 1099511628211;
#else
    const NUINT offset_basis = 2166136261;
    const NUINT prime = 16777619;
#endif

    NUINT hash = offset_basis;

    union _input
    {
        NUINT v;
        byte b[sizeof(NUINT)];
    };

    _input input;
    input.v = value ^ seed; // no clue where I should put seed

    hash = (hash ^ input.b[0]) * prime;
    hash = (hash ^ input.b[1]) * prime;
    hash = (hash ^ input.b[2]) * prime;
    hash = (hash ^ input.b[3]) * prime;
#ifdef SHOCK_64BIT
    hash = (hash ^ input.b[4]) * prime;
    hash = (hash ^ input.b[5]) * prime;
    hash = (hash ^ input.b[6]) * prime;
    hash = (hash ^ input.b[7]) * prime;
#endif

    return hash;
}

inline NUINT Hash(NUINT value)
{
    return Hash(value, HashDefault);
}

inline NUINT Hash(const char* c)
{
    return Hash((NUINT)c);
}

inline NUINT Hash(const void* v)
{
    return Hash((NUINT)v);
}

#endif // HASH_H_