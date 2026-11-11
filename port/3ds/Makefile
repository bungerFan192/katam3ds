# 3DS platform adapter smoke-test target. Original GBA game sources are not
# linked yet; see README.md for the remaining memory/ABI integration work.
.SUFFIXES:
TARGET := katam3ds-port
CFILES := main.c platform.c task_runtime.c clock_3ds.c save_3ds.c \
          wireless_3ds.c video_3ds.c audio_3ds.c
OBJS := $(CFILES:.c=.o)

DEVKITPRO ?= /opt/devkitpro
DEVKITARM ?= $(DEVKITPRO)/devkitARM
CC := $(DEVKITARM)/bin/arm-none-eabi-gcc

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CTRULIB := $(DEVKITPRO)/libctru
PORTLIBS := $(DEVKITPRO)/portlibs/3ds

CFLAGS := -g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH) \
          -D__3DS__ -I. -I$(CTRULIB)/include -I$(PORTLIBS)/include
LDFLAGS := -specs=3dsx.specs -g $(ARCH) \
           -L$(CTRULIB)/lib -L$(PORTLIBS)/lib \
           -Wl,-Map,$(TARGET).map
LIBS := -lcitro2d -lcitro3d -lctru -lm

.PHONY: all clean
all: $(TARGET).3dsx

$(TARGET).3dsx: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).3dsx $(TARGET).map
