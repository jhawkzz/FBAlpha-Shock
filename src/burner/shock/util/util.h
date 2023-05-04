
// See License.md for license

#ifndef UTIL_H_
#define UTIL_H_

void  flushPrintf( const char *pStr, ... );

#ifndef _WIN32
char *strlwr    (char *str);
int   stricmp(char *s1, char *s2);
#endif

int   getExeDirectory( char *pFilePath, int size );

#endif
