/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     twlnmenu.c

  This source code is derived from "NandInitializer" in TwlIPL

  $Date:: 2024-02-04#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#include <twl.h>
#include <twl/os.h>
#include <stdlib.h>

#include "screen.h"
#include "DEMO.h"
#include "twlnmenu.h"
#include "application_jump_private.h"

#define ROUND_UP(value, alignment) \
	(((u32)(value) + (alignment-1)) & ~(alignment-1))

static u64 ROM_SRL_TID[1] =
{
	0x0003001134544E41
};

#define ROM_SRL_PATH	"rom:/TwlNmenu.srl"

BOOL WriteTwlNmenu(void)
{
    FSFile  file;	
    BOOL    open_is_ok;
	BOOL    read_is_ok;
	void* pTempBuf;
	u32 file_size;
	u32 alloc_size;
	BOOL result = TRUE;
	//PutSubScreen(0, 22, 0xf2, " Copying TwlNmenu... ");
    FS_InitFile(&file);
    open_is_ok = FS_OpenFile(&file, OS_TMP_APP_PATH);
	if (open_is_ok)
	{
		FS_CloseFile(&file);
    	//PutSubScreen(0, 22, 0xf1, " Error: NAND SRL already exists!");
		return FALSE;
	}
    FS_InitFile(&file);
    open_is_ok = FS_OpenFile(&file, ROM_SRL_PATH);
	if (!open_is_ok)
	{
    	//PutSubScreen(0, 22, 0xf1, " Error: can't open TwlNmenu! (1)");
		return FALSE;
	}

	file_size  = FS_GetFileLength(&file) ;
	alloc_size = ROUND_UP(file_size, 32) ;
	pTempBuf = OS_Alloc( alloc_size );
	SDK_NULL_ASSERT(pTempBuf);
	DC_InvalidateRange(pTempBuf, alloc_size);
	//PutSubScreen(0, 22, 0xf2, " Writing TwlNmenu to buffer ");
	read_is_ok = FS_ReadFile( &file, pTempBuf, (s32)file_size );
	if (!read_is_ok)
	{
	    //PutSubScreen(0, 22, 0xf1, " Error: can't write TwlNmenu! (1)");
		FS_CloseFile(&file);
		OS_Free(pTempBuf);
		return FALSE;
	}

	FS_CloseFile(&file);
	//PutSubScreen(0, 22, 0xf2, " Creating TwlNmenu in NAND ");
    if (!FS_CreateFile(OS_TMP_APP_PATH, FS_PERMIT_R | FS_PERMIT_W))
    {
        //PutSubScreen(0, 22, 0xf1, " Error: can't create NAND file! ");
		result = FALSE;
    }
    else
    {
		FS_InitFile(&file);
		//PutSubScreen(0, 22, 0xf2, " Opening TwlNmenu in NAND");
        open_is_ok = FS_OpenFileEx(&file, OS_TMP_APP_PATH, FS_FILEMODE_W);
        if (!open_is_ok)
        
        {
            //PutSubScreen(0, 22, 0xf1, " Error: can't open TwlNmenu! (2)");
			result = FALSE;
        }
        else if (FS_WriteFile(&file, pTempBuf, (s32)file_size) == -1)
        {
            //PutSubScreen(0, 22, 0xf1, " Error: can't write TwlNmenu! (2)");
			result = FALSE;
        }
        //PutSubScreen(0, 22, 0xf2, " Closing file TwlNmenu "); 
        //(void)FS_CloseFile(&file);
        // This isn't working no matter what I try. Commenting out for now, hopefully files on NAND don't need any write protection lol
    }
	//PutSubScreen(0, 22, 0xf2, " Freeing buffer ");
	OS_Free(pTempBuf);
	//PutSubScreen(0, 22, 0xf2, " Done writing TwlNmenu ");
	return result;
}

void JumpTwlNmenu(void) {
	LauncherBootFlags flag;
	flag.bootType = LAUNCHER_BOOTTYPE_NAND;
	flag.isValid = TRUE;
	flag.isLogoSkip = TRUE;
	flag.isInitialShortcutSkip = FALSE;
	flag.isAppLoadCompleted = FALSE;
	flag.isAppRelocate = FALSE;
	flag.rsv = 0;
	//PutSubScreen(0, 22, 0xf2, " Jumping to TwlNmenu ");
	OS_SetLauncherParamAndResetHardware(ROM_SRL_TID[1], &flag);
	//PutSubScreen(0, 22, 0xf1, " Failed jump to TwlNmenu        ");
	OS_Terminate();
}

