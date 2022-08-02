.PHONY: all cleansub
all:
	mkdir -p iso
	mkpsxiso -y ./isoconfig.xml
cleansub:
	$(MAKE) clean
	rm -rf iso/

publish: iso/NeonCoaster.bin iso/NeonCoaster.cue
	butler push iso ioribranford/neon-coaster:psx-ntsc-uc --userversion ${VERSION}

TARGET = game
TYPE = ps-exe

SRCS = 	src/main.c \
		src/draw.c \
		src/image.c \
		src/input.c \
		src/time.c \
		src/audio.c \
		src/track.c \
		src/car.c \
		src/background.c \
../third_party/common/syscalls/printf.s \
../third_party/common/crt0/crt0.s \

CPPFLAGS += -Wall
CPPFLAGS += -Iinclude
CPPFLAGS += -I../third_party/common
CPPFLAGS += -I../third_party/psyq/include
LDFLAGS += -L../third_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgpu
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -lcd
LDFLAGS += -Wl,--end-group

include ../third_party/common.mk
