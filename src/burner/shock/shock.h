
// See License.md for license

#ifndef SHOCK_H_
#define SHOCK_H_

#include "shock/systems.h"
#include "burner.h"

#define SHOCK_VERSION "0.1.1"

// Before a game is loaded, we sometimes need to provide certain systems
// with an FPS to start with. Use 60.
#define DEFAULT_FPS (6000)

// Although games could theoretically support more, we'll only ever support 2
#define MAX_SHOCK_PLAYERS (2)

#ifdef MVSX
#define ASSET_ROOT_PATH     "/mnt/hdisk/fbashock"
#elif ASP
#define ASSET_ROOT_PATH     "/mnt/usb/fbashock"
#else // not used on win32 or lubuntu
#define ASSET_ROOT_PATH     ""
#endif

// IO for FBA to write game files
#define EEPROM_PATH          "eeprom/" 
#define BLEND_PATH           "blend/"
#define HISCORE_PATH         "hiscore/"
#define HISCORE_DAT_FILENAME "hiscore.dat"

// The size of the thumbnail images for save states
#define STATE_THUMBNAIL_WIDTH  (150)
#define STATE_THUMBNAIL_HEIGHT (120)

// Graphics
#define FRAMEBUFFER_MAX_WIDTH    (1280)
#ifdef ASP
#define FRAMEBUFFER_MAX_HEIGHT   (720)
#else
#define FRAMEBUFFER_MAX_HEIGHT   (1024)
#endif
#define FRAMEBUFFER_BPP	         (16)
#define FRAMEBUFFER_BYTES_PP     (FRAMEBUFFER_BPP / 8)
//

// UI
#define UI_WIDTH  (FRAMEBUFFER_MAX_WIDTH)
#define UI_HEIGHT (FRAMEBUFFER_MAX_HEIGHT)
//

// Audio
#define SAMPLE_BITS_PER_SAMPLE (16)
#define SAMPLE_NUM_CHANNELS    (2)
#define SAMPLE_RATE            (44100)
#define SAMPLE_SIZE            (SAMPLE_BITS_PER_SAMPLE / 8)
#define SAMPLE_BLOCK_ALIGN     (SAMPLE_SIZE * SAMPLE_NUM_CHANNELS)
#define MAX_AUDIO_BUFFER_BYTES (1024 * 1024)
//

// Time Conversion
#define SEC_TO_NANOSECONDS     (1000000000)
#define SEC_TO_MICROSECONDS    (1000000)
#define SEC_TO_MILLISECONDS    (1000)

#define NANO_TO_MICROSECONDS   (1000)

#define MILLI_TO_MICROSECONDS  (1000)
//

#ifndef _WIN32
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif


class OSTimer;
extern OSTimer gGlobalTimer;

#endif
