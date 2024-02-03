/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     main.c

  This source code is derived from "os_jump" in the TwlSDK

  $Date:: 2024-02-01#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#include <twl.h>
#include <stdlib.h>

#include "screen.h"
#include "DEMO.h"
#include "twlnmenu.h"

static u64 ROM_SRL_TID[1] =
{
	0x0003001134544E41
};
static const char ROM_SRL_PATH[1][32] = 
{
	"rom:/TwlNmenu.srl"
};


static BOOL WriteTwlNmenu(void)
{
    FSFile  file;	
    BOOL    open_is_ok;
	BOOL    read_is_ok;
	void* pTempBuf;
	u32 file_size;
	u32 alloc_size;
	BOOL result = TRUE;

    FS_InitFile(&file);
    open_is_ok = FS_OpenFile(&file, OS_TMP_APP_PATH);
	if (open_is_ok)
	{
		FS_CloseFile(&file);
    	OS_Printf("%s is already exist.", OS_TMP_APP_PATH);
		return TRUE;
	}

    FS_InitFile(&file);
    open_is_ok = FS_OpenFile(&file, ROM_SRL_PATH);
	if (!open_is_ok)
	{
    	OS_Printf("FS_OpenFile(%s) ERROR!", ROM_SRL_PATH);
		return FALSE;
	}

	file_size  = FS_GetFileLength(&file) ;
	alloc_size = ROUND_UP(file_size, 32) ;
	pTempBuf = OS_Alloc( alloc_size );
	SDK_NULL_ASSERT(pTempBuf);
	DC_InvalidateRange(pTempBuf, alloc_size);
	read_is_ok = FS_ReadFile( &file, pTempBuf, (s32)file_size );
	if (!read_is_ok)
	{
	    OS_Panic("FS_ReadFile(%s) ERROR!", ROM_SRL_PATH);
		FS_CloseFile(&file);
		OS_Free(pTempBuf);
		return FALSE;
	}

	FS_CloseFile(&file);

    if (!FS_CreateFile(OS_TMP_APP_PATH, FS_PERMIT_R | FS_PERMIT_W))
    {
        OS_Panic("FS_CreateFile(%s) failed.", OS_TMP_APP_PATH);
		result = FALSE;
    }
    else
    {
		FS_InitFile(&file);
        open_is_ok = FS_OpenFileEx(&file, OS_TMP_APP_PATH, FS_FILEMODE_W);
        if (!open_is_ok)
        {
            OS_Panic("FS_OpenFile(%s) failed.\n", OS_TMP_APP_PATH);
			result = FALSE;
        }
        else if (FS_WriteFile(&file, pTempBuf, (s32)file_size) == -1)
        {
            OS_Panic("FS_WritFile() failed.\n");
			result = FALSE;
        }
        (void)FS_CloseFile(&file);
    }

	OS_Free(pTempBuf);

	return result;
}

static void JumpTwlNmenu(void) {
	OS_DoApplicationJump( ROM_SRL_TID[0], OS_APP_JUMP_TMP );
	PutSubScreen(0, 22, 0xf1, " Failed jump to TwlNmenu        ");
}


