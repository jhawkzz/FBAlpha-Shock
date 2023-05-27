
// See License.md for license

#include "shock/util/util.h"

void flushPrintf( const char *pStr, ... )
{
	va_list argptr;
    va_start( argptr, pStr );
	vprintf( pStr, argptr );

#ifdef _WIN32
    char text[256] = {};
    vsnprintf(text, sizeof(text) - 1, pStr, argptr);
    OutputDebugString(text);
    OutputDebugString("\n");
#endif

    fflush(stdout);
	va_end( argptr );

}

#ifndef _WIN32
char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) 
  {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

// Taken from: https://www.geeksforgeeks.org/write-your-own-strcmp-which-ignores-cases/
int stricmp(char *s1, char *s2)
{
    int i;
    for (i = 0; s1[i] && s2[i]; ++i)
    {   
        /* If characters are same or inverting the
           6th bit makes them same */
        if (s1[i] == s2[i] || (s1[i] ^ 32) == s2[i])
           continue;
        else
           break;
    }
 
    /* Compare the last (or first mismatching in
       case of not same) characters */
    if (s1[i] == s2[i])
        return 0;
 
    // Set the 6th bit in both, then compare
    if ((s1[i] | 32) < (s2[i] | 32))
        return -1;
    return 1;
}

int getExeDirectory( char *pFilePath, int size )
{
    char result[ MAX_PATH ] = { 0 };
    ssize_t count = readlink("/proc/self/exe", result, MAX_PATH );
    if ( count != -1 ) 
    {
        dirname( result );

        snprintf( pFilePath, size, "%s", result );
        return 0;
    }
    else
    {
        return -1;
    }
}

int getAssetDirectory(char* pFilePath, int size)
{
    snprintf(pFilePath, size, "%s", ASSET_ROOT_PATH);
    return 0;
}

#else
int getAssetDirectory(char* pFilePath, int size)
{
    return getExeDirectory(pFilePath, size);
}

int getExeDirectory( char *pFilePath, int size )
{
    if (GetModuleFileName(NULL, pFilePath, size) == ERROR_INSUFFICIENT_BUFFER)
        return -1;

    char* slash = strrchr(pFilePath, '\\');
    if (slash) *slash = 0;

    return 0;
}

#endif
