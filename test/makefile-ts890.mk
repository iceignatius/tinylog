#
# Tiny Logger Library - Test
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
OUTPUT_TYPE = app
TEMP_DIR    = temp

INCDIR  :=
INCDIR  += -I../submod/ts890sdkfix
INCDIR  += -I..
LDDIR   :=
LDDIR   += -L..
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
LIBS    += -ltlog

SRCS :=
SRCS += main.c


################################################################################
# Build process
################################################################################

.PHONY: all clean install
.PHONY: pre_step build_step post_step

all: pre_step build_step post_step

include ../submod/ts890sdkfix/makeutil.mak

pre_step:

build_step: uicdev_all

post_step:

clean: uicdev_clean

install: uicdev_install
