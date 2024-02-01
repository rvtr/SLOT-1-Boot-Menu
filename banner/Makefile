#---------------------------------------------------------------------------
# Project:  SLOT-1 Boot Menu
# File:     Makefile (banner)
#
# This source code is derived from "os_jump" in the TwlSDK
#
# $Date:: 2024-02-01#$
# $Author: Lillian Skinner (rmc) $
#---------------------------------------------------------------------------

TARGET_PLATFORM	 = TWL

include $(TWLSDK_ROOT)/build/buildtools/commondefs

BANNER_ICON		 = banner.bmp
BANNER_SPEC		 = banner.bsf

TARGETS			 = $(BANNER_SPEC:.bsf=.bnr)

BANNER_ICON_NAME = $(basename $(BANNER_ICON))
BANNER_ICON_MIDDLE	= $(addprefix $(BANNER_ICON_NAME), .nbfs .nbfc .nbfp)
MAKEBANNER_FLAGS	= -N $(BANNER_ICON_NAME)

LDIRT_CLEAN		 = $(TARGETS) \
				   $(BANNER_ICON_MIDDLE) \

#----------------------------------------------------------------------------

do-build:		$(BANNER_ICON_MIDDLE) $(TARGETS)

#----------------------------------------------------------------------------
include $(TWLSDK_ROOT)/build/buildtools/modulerules


# Breaks too often, just do this manually and edit as needed:
#	/cygdrive/c/TwlSDK_5.5/tools/bin/ntexconv.exe -no -bg -bgb -bgnc -ta -f palette16 banner.bmp
#	/cygdrive/c/TwlSDK_5.5/tools/bin/makebanner.TWL.exe -N banner banner.bsf banner.bnr
