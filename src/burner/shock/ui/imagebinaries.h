
// See License.md for license

#ifndef IMAGEBINARY_H_
#define IMAGEBINARY_H_

#include "shock/shock.h"

// 565 16bit - NOTE: We're getting lucky that the stride and pitch match.
// If we run into pitch issues, we need to factor in stride. (See font.h for that issue)
extern UINT16 gBlueButtonBytes[2751];
extern UINT16 gGreenButtonBytes[2751];
extern UINT16 gRedButtonBytes[2751];
extern UINT16 gWhiteMButtonMVSXBytes[2751];
extern UINT16 gWhiteRButtonMVSXBytes[2751];
extern UINT16 gWhiteTButtonASPBytes[ 2751 ];
extern UINT16 gWhiteBButtonASPBytes[ 2751 ];
extern UINT16 gGrayTButtonASPBytes[ 2751 ];
extern UINT16 gGrayBButtonASPBytes[ 2751 ];
extern UINT16 gYellowButtonBytes[2751];
extern UINT16 gMainBGHeader[229121];

#endif
