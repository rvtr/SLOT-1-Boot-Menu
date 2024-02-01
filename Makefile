#! make -f
#---------------------------------------------------------------------------
# Project:  SLOT-1 Boot Menu
# File:     Makefile
#
# This source code is derived from "os_jump" in the TwlSDK
#
# $Date:: 2024-02-01#$
# $Author: Lillian Skinner (rmc) $
#---------------------------------------------------------------------------

# SUBDIRS		= banner \

TARGET_PLATFORM := TWL

TWL_ARCHGEN		= LIMITED

TARGET_BIN	= SLOT1_Booter.srl

SRCS		= main.c screen.c font.c

ROM_SPEC	= specfile.rsf

#---------------------------------------------------------------------------

include $(TWLSDK_ROOT)/build/buildtools/commondefs
include	$(TWLSDK_ROOT)/build/buildtools/commondefs.gx.demolib

# MAKEROM		= $(TWLSDK_ROOT)/tools/bin/makerom.TWL.secure.exe

do-build:	$(TARGETS)

include	$(TWLSDK_ROOT)/build/buildtools/modulerules
