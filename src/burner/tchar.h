#ifndef __PORT_TYPEDEFS_H
#define __PORT_TYPEDEFS_H

#include <stdint.h>
#include <wchar.h>

#define TCHAR char
#define _T(x) x
#define _tfopen fopen
#define _tcstol strtol
#define _tcsstr strstr
#define _istspace(x) isspace(x)
#define _stprintf sprintf
#define _tcslen strlen
#define _tcsicmp(a, b) strcasecmp(a, b)
#define _tcscpy(to, from) strcpy(to, from)
#define _fgetts fgets
#define _strnicmp(s1, s2, n) strncasecmp(s1, s2, n)
#define _tcsncmp strncmp
#define _tcsncpy strncpy
#define _stscanf sscanf
#define _ftprintf fprintf

#define _stricmp(x, y) strcasecmp(x,y)

#ifndef _WIN32
typedef struct { int x, y, width, height; } RECT;
#endif

#undef __cdecl
#define __cdecl

#define bprintf(...) {}

#undef __fastcall
#undef _fastcall
#define __fastcall			/*what does this correspond to?*/
#define _fastcall			/*same as above - what does this correspond to?*/
#define ANSIToTCHAR(str, foo, bar) (str)

/*FBA defines*/
#define PUF_TEXT_NO_TRANSLATE	(0)
#define PUF_TYPE_ERROR		(1)

#endif