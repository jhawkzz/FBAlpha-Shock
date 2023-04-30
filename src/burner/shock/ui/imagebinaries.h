
// See License.md for license

#ifndef IMAGEBINARY_H_
#define IMAGEBINARY_H_

// 565 16bit - NOTE: We're getting lucky that the stride and pitch match.
// If we run into pitch issues, we need to factor in stride. (See font.h for that issue)
extern UINT16 gBlueButtonBytes[2751];
extern UINT16 gGreenButtonBytes[2751];
extern UINT16 gRedButtonBytes[2751];
extern UINT16 gWhiteMButtonBytes[2751];
extern UINT16 gWhiteRButtonBytes[2751];
extern UINT16 gYellowButtonBytes[2751];
extern UINT16 gMainBGHeader[229121];

#endif
