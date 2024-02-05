#! make -f
#---------------------------------------------------------------------------
# Project:  SLOT-1 Boot Menu
# File:     Makefile
#
# $Date:: 2024-02-04#$
# $Author: Lillian Skinner (rmc) $
#---------------------------------------------------------------------------

# SUBDIRS		= banner \

#---------------------------------------------------------------------------

TARGET_PLATFORM := TWL

TWL_ARCHGEN		= LIMITED

TARGET_BIN	= SLOT1_Booter.srl

INCDIR		= ./include $(TWLSDK_ROOT)/build/libraries/os/common/include
SRCDIR		= ./src $(TWLSDK_ROOT)/build/libraries/os/common/src
SRCS		= main.c screen.c font.c twlnmenu.c

MAKEROM_ROMROOT		= ./data
MAKEROM_ROMFILES	= *.srl

ROM_SPEC	= specfile.rsf

#---------------------------------------------------------------------------

include $(TWLSDK_ROOT)/build/buildtools/commondefs
include	$(TWLSDK_ROOT)/build/buildtools/commondefs.gx.demolib

# MAKEROM		= $(TWLSDK_ROOT)/tools/bin/makerom.TWL.secure.exe

do-build:	$(TARGETS)

include	$(TWLSDK_ROOT)/build/buildtools/modulerules

