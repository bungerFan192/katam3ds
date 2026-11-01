# Standalone smoke-test target; does not yet compile the original game sources.
.SUFFIXES:
TARGET := katam3ds-port
CFILES := main.c platform.c task_runtime.c
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
LIBS := -lcitro2d -lcitro3d -lctru

.PHONY: all clean
all: $(TARGET).3dsx

$(TARGET).3dsx: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).3dsx $(TARGET).map
