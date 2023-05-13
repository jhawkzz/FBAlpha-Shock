#ifndef PLATFORM_H_
#define PLATFORM_H_

#if defined X86 || defined MVSX
   #define ShockSleep(us) usleep(us)
    #define ShockCreateDir(path) mkdir( (path), 0777 )
#elif defined _WIN32
   #define ShockSleep(us) Sleep((us) / 1000)
    #define ShockCreateDir(path) CreateDirectory( (path), NULL )
#endif

#endif