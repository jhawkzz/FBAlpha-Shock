
// See License.md for license

#ifndef UTIL_H_
#define UTIL_H_

void  flushPrintf( const char *pStr, ... );

#ifndef _WIN32
char *strlwr    (char *str);
int   stricmp(char *s1, char *s2);
#endif

int   getExeDirectory( char *pFilePath, int size );

#if defined LUBUNTU || defined MVSX || defined ASP
    #define ShockSleep(us) usleep(us)
    #define ShockCreateDir(path) mkdir( (path), 0777 )
#elif defined _WIN32
    #define ShockSleep(us) Sleep((us) / 1000)
    #define ShockCreateDir(path) CreateDirectory( (path), NULL )
#endif

#endif
