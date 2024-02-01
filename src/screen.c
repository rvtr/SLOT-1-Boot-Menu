/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     screen.c

  This source code is derived from "os_jump" in the TwlSDK

  $Date:: 2024-02-01#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include "screen.h"
#include "font.h"

/*---------------------------------------------------------------------------*
    Constant Definitions
 *---------------------------------------------------------------------------*/
#define TEXT_SCREEN_SIZE    2048

/*---------------------------------------------------------------------------*
    Internal Variable Definitions
 *---------------------------------------------------------------------------*/

// Virtual screen [top and bottom screens][number of backgrounds][number of characters]
static u16  gScreen[2 ][ 1 ][ TEXT_SCREEN_SIZE / sizeof(u16) ] ATTRIBUTE_ALIGN(32);

/*---------------------------------------------------------------------------*
  Name:         InitScreen

  Description:  Initialize display settings for text display system.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void InitScreen(void)
{
    // Various VRAM initialization steps
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void*)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    (void)GX_DisableBankForLCDC();

    // Initialize OAM
    MI_CpuFillFast((void*)HW_OAM, 0xc0, HW_OAM_SIZE);
    MI_CpuFillFast((void*)HW_DB_OAM, 0xc0, HW_DB_OAM_SIZE);

    // Initialize palette
    MI_CpuClearFast((void*)HW_PLTT, HW_PLTT_SIZE);
    MI_CpuClearFast((void*)HW_DB_PLTT, HW_DB_PLTT_SIZE);

    // Top screen settings
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    G2_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000,
                     GX_BG_EXTPLTT_01);
    G2_SetBG0Priority(0);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG0);
    GX_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GX_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    ((u16*)HW_PLTT)[0] = 0x0000;    // Black
    MI_CpuFillFast(gScreen[0][0], 0, TEXT_SCREEN_SIZE);
    DC_StoreRange(gScreen[0][0], TEXT_SCREEN_SIZE);
    GX_LoadBG0Scr(gScreen[0][0], 0, TEXT_SCREEN_SIZE);

    // Bottom screen settings
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    G2S_SetBG0Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000,
                      GX_BG_EXTPLTT_01);
    G2S_SetBG0Priority(0);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(GX_PLANEMASK_BG0);
    GXS_LoadBG0Char(d_CharData, 0, sizeof(d_CharData));
    GXS_LoadBGPltt(d_PaletteData, 0, sizeof(d_PaletteData));
    ((u16*)HW_DB_PLTT)[0] = 0x0000; // Black
    MI_CpuFillFast(gScreen[1][0], 0, TEXT_SCREEN_SIZE);
    DC_StoreRange(gScreen[1][0], TEXT_SCREEN_SIZE);
    GXS_LoadBG0Scr(gScreen[1][0], 0, TEXT_SCREEN_SIZE);
}

/*---------------------------------------------------------------------------*
  Name:         ClearScreen

  Description:  Clears text display from the screen.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void ClearScreen(void)
{
    MI_CpuClearFast(gScreen[0][0], TEXT_SCREEN_SIZE);
    MI_CpuClearFast(gScreen[1][0], TEXT_SCREEN_SIZE);
}
void ClearMainScreen(void)
{
    MI_CpuClearFast(gScreen[0][0], TEXT_SCREEN_SIZE);

}
void ClearSubScreen(void)
{
    MI_CpuClearFast(gScreen[1][0], TEXT_SCREEN_SIZE);
}
/*---------------------------------------------------------------------------*
  Name:         PutMainScreen

  Description:  Outputs text to main screen.

  Arguments:    text: String to be output
                ...: Virtual argument

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PutMainScreen(s32 x, s32 y, u8 palette, char* text, ...)
{
    va_list vlist;
    char    temp[33];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    for (i = 0; i < 32; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }

        gScreen[0][0][((y * 32) + x + i) % (32 * 32)] = (u16) (palette << 12 | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         PrintSubScreen

  Description:  Outputs text to the sub-screen.

  Arguments:    text: String to be output
                ...: Virtual argument

  Returns:      None.
 *---------------------------------------------------------------------------*/
void PutSubScreen(s32 x, s32 y, u8 palette, char* text, ...)
{
    va_list vlist;
    char    temp[33];
    s32     i;

    va_start(vlist, text);
    (void)vsnprintf(temp, 33, text, vlist);
    va_end(vlist);

    for (i = 0; i < 32; i++)
    {
        if (temp[i] == 0x00)
        {
            break;
        }

        gScreen[1][0][((y * 32) + x + i) % (32 * 32)] = (u16) (palette << 12 | temp[i]);
    }
}

/*---------------------------------------------------------------------------*
  Name:         UpdateScreen

  Description:  Reflects the virtual screen content to VRAM.
                Assumed to be called during the V-Blank period.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void UpdateScreen(void)
{
    // Reflects the virtual screen content to V-RAM
    DC_StoreRange(gScreen[0][0], TEXT_SCREEN_SIZE);
    GX_LoadBG0Scr(gScreen[0][0], 0, TEXT_SCREEN_SIZE);
    DC_StoreRange(gScreen[1][0], TEXT_SCREEN_SIZE);
    GXS_LoadBG0Scr(gScreen[1][0], 0, TEXT_SCREEN_SIZE);
}
