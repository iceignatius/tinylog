#
# Tiny Logger Library
#

################################################################################
# Environment settings
################################################################################

HW      = netarm9
HW_VER  = 010
OS      = lpos
OS_VER  = 200
LIB     = lib
LIB_VER = 102


################################################################################
# User defined settings
################################################################################

PROJECT     = tlog
OUTPUT_TYPE = lib
TEMP_DIR    = temp

INCDIR  :=
INCDIR  += -Isubmod/ts890sdkfix
INCDIR  += -Isubmod/genutil
LDDIR   :=
DEFINE  :=
DEFINE  += -Dnetarm9
DEFINE  += -DUIC_TS890
DEFINE  += -DAC_USE_SYSTEM_EXTENSIONS
DEFINE  += -DNO_WCHAR
CFLAGS  :=
CFLAGS  += -Wall
CFLAGS  += -std=gnu99
CFLAGS  += -pthread
CFLAGS  += -Os
LDFLAGS :=
LDFLAGS += -pthread
LDFLAGS += -s
LIBS    :=

SRCS :=
SRCS += submod/genutil/gen/systime.c
SRCS += submod/genutil/gen/timeinf.c
SRCS += tlog.c


################################################################################
# Build process
################################################################################

.PHONY: all clean install
.PHONY: pre_step build_step post_step

all: pre_step build_step post_step
	cd test && $(MAKE) -f makefile-ts890.mk $(MAKECMDGOALS)

include submod/ts890sdkfix/makeutil.mak

pre_step:

build_step: uicdev_all

post_step:

clean: uicdev_clean
	cd test && $(MAKE) -f makefile-ts890.mk $(MAKECMDGOALS)

install: uicdev_install
