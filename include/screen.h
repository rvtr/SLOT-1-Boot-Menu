/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     screen.h

  This source code is derived from "os_jump" in the TwlSDK

  $Date:: 2024-02-01#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#ifndef SCREEN_H_
#define SCREEN_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

/*---------------------------------------------------------------------------*
    Function Definitions
 *---------------------------------------------------------------------------*/
void    InitScreen(void);
void    ClearScreen(void);
void    ClearMainScreen(void);
void    ClearSubScreen(void);
void    PutMainScreen(s32 x, s32 y, u8 palette, char* text, ...);
void    PutSubScreen(s32 x, s32 y, u8 palette, char* text, ...);
void    UpdateScreen(void);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* SCREEN_H_ */
