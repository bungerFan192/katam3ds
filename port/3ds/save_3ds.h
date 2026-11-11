#ifndef KATAM3DS_SAVE_3DS_H
#define KATAM3DS_SAVE_3DS_H
#include <stddef.h>
#include <stdint.h>
int K3D_SaveWrite(const char *name, const void *data, size_t size);
int K3D_SaveRead(const char *name, void *data, size_t capacity, size_t *read_size);
#endif
