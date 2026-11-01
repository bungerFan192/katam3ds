# Standalone smoke-test target; does not yet compile the original game sources.
.SUFFIXES:
TARGET := katam3ds-port
BUILD := build
SOURCES := .
INCLUDES := -I.
CFILES := main.c platform.c task_runtime.c
OBJS := $(CFILES:.c=.o)
ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
CFLAGS := -g -Wall -O2 -mword-relocations -ffunction-sections $(ARCH) $(INCLUDES)
LDFLAGS := -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(TARGET).map
LIBS := -lcitro2d -lcitro3d -lctru
.PHONY: all clean
all: $(TARGET).3dsx
$(TARGET).3dsx: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).3dsx $(TARGET).map
