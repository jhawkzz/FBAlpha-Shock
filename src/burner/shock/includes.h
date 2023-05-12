
// See License.md for license

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <linux/soundcard.h>
#include <string.h>
#include <linux/input.h>
#include <poll.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <dirent.h>
#include <linux/fb.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>

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

#ifdef X86
#include "shock/core/x86/x86framebuffer.h"
#elif MVSX
#include "shock/core/mvsx/mvsxframebuffer.h"
#endif

#ifdef X86
#include <alsa/asoundlib.h>
#include "shock/core/x86/x86audio.h"
#elif MVSX
#include "shock/core/mvsx/mvsxaudio.h"
#endif

#ifdef X86
#include "shock/core/x86/x86input.h"
#elif MVSX
#include "shock/core/mvsx/mvsxinput.h"
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
