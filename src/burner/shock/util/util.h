
// See License.md for license

#ifndef UTIL_H_
#define UTIL_H_

#include "shock/shock.h"

void  flushPrintf( const char *pStr, ... );

#ifndef _WIN32
char *strlwr    (char *str);
int   stricmp(char *s1, char *s2);
#endif

int   getExeDirectory( char *pFilePath, int size );

#if defined LUBUNTU || defined MVSX_ASP
    #define ShockSleep(us) usleep(us)
    #define ShockCreateDir(path) mkdir( (path), 0777 )
    #define SC_ASSERT(c) assert(c)
#elif defined _WIN32
    #define ShockSleep(us) Sleep((us) / 1000)
    #define ShockCreateDir(path) CreateDirectory( (path), NULL )
    #define SC_ASSERT(c) _ASSERTE(c)
#endif

#endif
