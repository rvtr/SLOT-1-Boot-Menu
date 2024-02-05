/*---------------------------------------------------------------------------*
  Project:  SLOT-1 Boot Menu
  File:     main.c

  This source code is derived from "os_jump" in the TwlSDK

  $Date:: 2024-02-03#$
  $Author: Lillian Skinner (rmc) $
 *---------------------------------------------------------------------------*/

#include <twl.h>
#include <stdlib.h>

#include "screen.h"
#include "DEMO.h"
#include "payload.h"
#include "twlnmenu.h"

/*---------------------------------------------------------------------------*
    Constant Definitions
 *---------------------------------------------------------------------------*/
#define VERSION				83
#define KEY_REPEAT_START    25  // Number of frames until key repeat starts
#define KEY_REPEAT_SPAN     10  // Number of frames between key repeats
#define DMA_NO_FS	1

/*---------------------------------------------------------------------------*
    Structure Definitions
 *---------------------------------------------------------------------------*/

// Key input data
typedef struct KeyInfo
{
    u16 cnt;    // Unprocessed input value
    u16 trg;    // Push trigger input
    u16 up;     // Release trigger input
    u16 rep;    // Press and hold repeat input
} KeyInfo;

// Key input
static KeyInfo  gKey;

BOOL result;
/*---------------------------------------------------------------------------*
    Payload stuffs
 *---------------------------------------------------------------------------*/

typedef unsigned char uu8;
typedef unsigned short uu16;

/*---------------------------------------------------------------------------*
   Prototype
 *---------------------------------------------------------------------------*/

static void VBlankIntr(void);
static void InitInterrupts(void);
static void InitHeap(void);

static void ReadKey(KeyInfo* pKey);

/*---------------------------------------------------------------------------*/
void VBlankIntr(void);

void TwlMain(void)
{
    OS_Init();
    OS_InitTick();
    OS_InitAlarm();

    // When in NITRO mode, stopped by Panic
    DEMOCheckRunOnTWL();

    GX_Init();
    GX_DispOff();
    GXS_DispOff();

    InitHeap();
    InitScreen();
    InitInterrupts();
    
    FS_Init(DMA_NO_FS);

    GX_DispOn();
    GXS_DispOn();

    ClearScreen();

    // Empty call for getting key input data (strategy for pressing A Button in the IPL)
    ReadKey(&gKey);

    while(TRUE)
    {
        // Get key input data
        ReadKey(&gKey);

        // Clear the main screen
        ClearScreen();
        
        PutMainScreen(0, 1, 0xff, " ------ SLOT-1 BOOT MENU ------ ");
        PutMainScreen(0, 3, 0xff, "  By rmc                        ");
        PutMainScreen(0, 4, 0xff, "  https://rvtr.github.io/       ");
        PutMainScreen(0, 6, 0xff, " ------------------------------ ");
        
        // 32 wide, 24 tall
        PutSubScreen(0, 0, 0xff, "  SLOT-1 BOOT MENU ver.%06d", VERSION);
        PutSubScreen(0, 2, 0xff, " -- KEY -------- COMMAND ------ ");
        PutSubScreen(0, 4, 0xf2, "    A:           Launcher       ");
        PutSubScreen(0, 6, 0xf2, "    B:           Settings       ");
        PutSubScreen(0, 8, 0xf2, "    X:           TwlNmenu       ");
        PutSubScreen(0, 10, 0xf2, "    Y:           SD (boot.nds)  ");
		PutSubScreen(0, 20, 0xff, " ------------------------------ ");
		PutSubScreen(0, 21, 0xff, " STATUS:                        ");
		PutSubScreen(0, 22, 0xff, " Currently no error occurs      ");

        
        if (gKey.trg & PAD_BUTTON_A)
        {
            if (FALSE == OS_JumpToSystemMenu() )
            {
                //Jump failure
                PutSubScreen(0, 22, 0xf1, " Failed jump to Launcher        ");
            }
        }

        if (gKey.trg & PAD_BUTTON_B)
        {
            if (FALSE == OS_JumpToWirelessSetting())
            // OSi_CanArbitraryJumpTo(0x00030015484e4241)
            {
                //Jump failure
                PutSubScreen(0, 22, 0xf1, " Failed jump to MachineSettings ");
            }
        }
        
        if (gKey.trg & PAD_BUTTON_X)
		{
            result = WriteTwlNmenu();
            if (result == TRUE)
            {
	            JumpTwlNmenu();
            }

            PutSubScreen(0, 22, 0xf1, " Something went very wrong! ");
            OS_Terminate();
		}

        if (gKey.trg & PAD_BUTTON_Y)
        {
        	// """Adapted""" from nathanfarlow's stylehax (adapted from shutterbug2000 and zoogie's memory pit)
            uu8 *payload_dest = (uu8 *)0x02200000;

			int i;
			
		    for (i=0; i < sizeof(mini_twl_payload); i++) {
    			payload_dest[i] = mini_twl_payload[i];
    		}
    		// Call payload
    		((void (*)())payload_dest)();
        }
        // Wait for V-Blank (this supports threading)
        OS_WaitVBlankIntr();
    }
    
    OS_WaitVBlankIntr();
    OS_Terminate();
}

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V-Blank interrupt handler.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void VBlankIntr(void)
{
    // Updates text display
    UpdateScreen();

    // Sets the IRQ check flag
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupts

  Description:  Initializes the interrupt settings.
                Allows V-Blank interrupts and configures the interrupt handler.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitInterrupts(void)
{
    // V-Blank interrupt settings
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);

    // Allow interrupts
    (void)OS_EnableIrq();
    (void)OS_EnableInterrupts();
}

/*---------------------------------------------------------------------------*
  Name:         InitHeap

  Description:  Initializes the memory allocation system within the main memory arena.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void InitHeap(void)
{
    void*           tempLo;
    OSHeapHandle    hh;

    // Creates a heap in the main memory arena
    tempLo = OS_InitAlloc(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi(), 1);
    OS_SetArenaLo(OS_ARENA_MAIN, tempLo);
    hh = OS_CreateHeap(OS_ARENA_MAIN, OS_GetMainArenaLo(), OS_GetMainArenaHi());
    if (hh < 0)
    {
        // Abnormal end when heap creation fails
        OS_Panic("ARM9: Fail to create heap...\n");
    }
    (void)OS_SetCurrentHeap(OS_ARENA_MAIN, hh);
}

/*---------------------------------------------------------------------------*
  Name:         ReadKey

  Description:  Gets key input data and edits the input data structure.
                Detects push, release, and push and hold repeat triggers.

  Arguments:    pKey: Key input structure to be changed

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ReadKey(KeyInfo* pKey)
{
    static u16  repeat_count[12];
    int         i;
    u16         r;

    r = PAD_Read();
    pKey->trg = 0x0000;
    pKey->up = 0x0000;
    pKey->rep = 0x0000;

    for (i = 0; i < 12; i++)
    {
        if (r & (0x0001 << i))
        {
            if (!(pKey->cnt & (0x0001 << i)))
            {
                pKey->trg |= (0x0001 << i);     // Press trigger
                repeat_count[i] = 1;
            }
            else
            {
                if (repeat_count[i] > KEY_REPEAT_START)
                {
                    pKey->rep |= (0x0001 << i); // Press-and-hold repeat
                    repeat_count[i] = (u16) (KEY_REPEAT_START - KEY_REPEAT_SPAN);
                }
                else
                {
                    repeat_count[i]++;
                }
            }
        }
        else
        {
            if (pKey->cnt & (0x0001 << i))
            {
                pKey->up |= (0x0001 << i);      // Release trigger
            }
        }
    }

    pKey->cnt = r;  // Unprocessed key input
}
