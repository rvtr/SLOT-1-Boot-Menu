/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     font.h

  This source code is derived from "os_jump" in the TwlSDK

  $Date:: 2024-02-01#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#ifndef FONT_H_
#define FONT_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

extern const u32    d_CharData[8 * 256];
extern const u32    d_PaletteData[8 * 16];

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* FONT_H_ */
