
// See License.md for license

#ifndef SHOCK_H_
#define SHOCK_H_

#include "shock/systems.h"
#include "burner.h"

#define SHOCK_VERSION "1.0.2 (Build 2)"

typedef unsigned char byte;

#ifdef _WIN64
    #define SHOCK_64BIT
#else
    #define SHOCK_32BIT
#endif

#ifdef SHOCK_64BIT
    typedef unsigned long long NUINT;
#else
    typedef unsigned int NUINT;
#endif

const unsigned int NUINT_SIZE = sizeof(NUINT);
const unsigned int VOIDPTR_SIZE = sizeof(void*);

#if (NUINT_SIZE != VOIDPTR_SIZE)
    #error Size Mismatch
#endif

// Before a game is loaded, we sometimes need to provide certain systems
// with an FPS to start with. Use 60.
#define DEFAULT_FPS (6000)

//#define SHOCK_PROFILERS

#ifdef SHOCK_PROFILERS
    #define SHOCK_PROFILERS_NEWFRAME\
            ShockProfilersDisplay::Capture();\
            ShockProfilers::Clear()
#else
    #define SHOCK_PROFILERS_NEWFRAME
#endif


// Although games could theoretically support more, we'll only ever support 2
#define MAX_SHOCK_PLAYERS (2)

#define MVSX_ASP_ASSET_ROOT_PATH "/mnt/hdisk/fbashock"

// IO for FBA to write game files
#define EEPROM_PATH          "eeprom/" 
#define BLEND_PATH           "blend/"
#define HISCORE_PATH         "hiscore/"
#define HISCORE_DAT_FILENAME "hiscore.dat"

// The size of the thumbnail images for save states
#define STATE_THUMBNAIL_WIDTH  (150)
#define STATE_THUMBNAIL_HEIGHT (120)

// Graphics
#define RESOLUTION_1280_WIDTH   (1280) //The highest resolution we'll use for rendering the game. Based on the MVSX's display (1280x1024)
#define RESOLUTION_960_WIDTH    (960) // asp's game width
#define RESOLUTION_1024_HEIGHT  (1024)
#define RESOLUTION_720_HEIGHT   (720) // Used when rendering to fullscreen (with stretch). Results in the best balance between visual quality and fps
#define FRAMEBUFFER_BPP	        (16)
#define FRAMEBUFFER_BYTES_PP    (FRAMEBUFFER_BPP / 8)
//

// UI
#define UI_WIDTH  (RESOLUTION_1280_WIDTH)
#define UI_HEIGHT (RESOLUTION_1024_HEIGHT)
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

enum ActivePlatform
{
    ActivePlatform_MVSX,
    ActivePlatform_ASP,
    ActivePlatform_Win32,
    ActivePlatform_Lubuntu,
    ActivePlatform_Count
};

extern ActivePlatform gActivePlatform;
extern char           gAssetPath[ MAX_PATH ];

#endif