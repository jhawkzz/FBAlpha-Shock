// See License.md for license

#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <io.h>
    #define _USE_MATH_DEFINES
    #include <math.h>
#else
    #include <limits.h>
    #include <unistd.h> //JHM: Fix implicit declaration warnings
    #include <sys/ioctl.h>
    #include <sys/mman.h>
    #include <sys/wait.h>
    #include <libgen.h>
    #include <pthread.h>
    #include <linux/soundcard.h>
    #include <linux/input.h>
    #include <poll.h>
    #include <sys/mount.h>
    #include <sys/reboot.h>
    #include <dirent.h>
    #include <linux/fb.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tchar.h>
#include <ctype.h>

#ifdef LUBUNTU
    #include <alsa/asoundlib.h>
#endif

#endif
