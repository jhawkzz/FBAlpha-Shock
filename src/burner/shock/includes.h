
// See License.md for license

#ifndef INCLUDES_H_
#define INCLUDES_H_

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <io.h>
    #define _USE_MATH_DEFINES
    #include <math.h>
    #define INLINE inline
#else
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

// FBA Includes
#include "burner.h"

// Shock Includes
#include "defines.h"
#include "shock/util/util.h"
#include "shock/util/ringbuffer.h"
#include "core/ostimer.h"

#ifdef MVSX
#include "shock/core/mvsx/mvsxled.h"
#endif

#ifdef LUBUNTU
#include <alsa/asoundlib.h>
#endif

#include "shock/shockrenderer.h"
#include "shock/shockaudio.h"
#include "shock/shockromloader.h"

#include "shock/input/shockinput.h"
#include "shock/input/shockburninput.h"
#include "shock/input/shockplayerinput.h"

#include "shock/shockconfig.h"

#include "shock/font/font.h"

#include "shock/ui/render/uirenderer.h"
#include "shock/ui/render/menuitem.h"
#include "shock/ui/render/drawlistobject.h"

#include "shock/ui/imagebinaries.h"
#include "shock/ui/states/uibasestate.h"
#include "shock/ui/states/stateloading.h"
#include "shock/ui/states/stateloaderror.h"
#include "shock/ui/states/statecredits.h"
#include "shock/ui/states/statemainmenu.h"
#include "shock/ui/states/stategamesettings.h"
#include "shock/ui/states/stateemulatorsettings.h"
#include "shock/ui/states/statedisplaysettings.h"
#include "shock/ui/states/statebuttonconfig.h"

#include "shock/hiscoredat.h"

#include "shock/ui/shockui.h"
#include "shock/shockgame.h"
#include "shock/shockmain.h"

#endif
