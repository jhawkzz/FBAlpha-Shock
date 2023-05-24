
// See License.md for license

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include "systems.h"

// FBA Includes
#include "burner.h"

// Shock Includes
#include "defines.h"
#include "shock/util/util.h"
#include "shock/util/ringbuffer.h"
#include "core/ostimer.h"

#ifdef LUBUNTU
#include <alsa/asoundlib.h>
#endif

#include "shock/shockrenderer.h"
#include "shock/shockaudio.h"
#include "shock/shockromloader.h"

#include "shock/input/shockburninput.h"

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
