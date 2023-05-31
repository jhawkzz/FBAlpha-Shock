
// See License.md for license

#ifndef SHOCK_H_
#define SHOCK_H_

#include "shock/systems.h"
#include "burner.h"

#define SHOCK_VERSION "0.1.0"

// Before a game is loaded, we sometimes need to provide certain systems
// with an FPS to start with. Use 60.
#define DEFAULT_FPS (6000)

// Although games could theoretically support more, we'll only ever support 2
#define MAX_SHOCK_PLAYERS (2)

// IO for FBA to write game files
#define EEPROM_PATH          "eeprom/" 
#define BLEND_PATH           "blend/"
#define HISCORE_PATH         "hiscore/"
#define HISCORE_DAT_FILENAME "hiscore.dat"

// Graphics
#ifdef LUBUNTU
#define PLATFORM_LCD_WIDTH	  (800)
#define PLATFORM_LCD_HEIGHT	  (600) 
#elif MVSX
#define PLATFORM_LCD_WIDTH	  (1280)
#define PLATFORM_LCD_HEIGHT	  (1024)
#elif ASP
#define PLATFORM_LCD_WIDTH	  (1280)
#define PLATFORM_LCD_HEIGHT	  (720)
#elif _WIN32
#define PLATFORM_LCD_WIDTH	  (1280)
#define PLATFORM_LCD_HEIGHT	  (1024)
#endif

#define PLATFORM_SCREEN_BPP	  (16)
#define PLATFORM_SCREEN_PITCH (PLATFORM_LCD_WIDTH * (PLATFORM_SCREEN_BPP / 8))
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
#endif

class OSTimer;
extern OSTimer gGlobalTimer;
