#include "save_3ds.h"
#include <stdio.h>
#include <string.h>

#define K3D_SAVE_DIR "sdmc:/3ds/katam3ds/"

static int MakePath(char *out, size_t capacity, const char *name)
{
    size_t i, n;
    if (!out || !name || !name[0] || capacity < 32)
        return 0;
    n = strlen(name);
    if (n > capacity - sizeof(K3D_SAVE_DIR))
        return 0;
    /* Save names are filenames, never paths. */
    for (i = 0; i < n; ++i) {
        unsigned char c = (unsigned char)name[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') || c == '_' || c == '-'))
            return 0;
    }
    strcpy(out, K3D_SAVE_DIR);
    strcat(out, name);
    return 1;
}

int K3D_SaveWrite(const char *name, const void *data, size_t size)
{
    char path[128];
    FILE *f;
    size_t written;
    if ((!data && size) || !MakePath(path, sizeof(path), name))
        return 0;
    (void)mkdir("sdmc:/3ds", 0777);
    (void)mkdir(K3D_SAVE_DIR, 0777);
    f = fopen(path, "wb");
    if (!f)
        return 0;
    written = size ? fwrite(data, 1, size, f) : 0;
    if (fclose(f) != 0)
        return 0;
    return written == size;
}

int K3D_SaveRead(const char *name, void *data, size_t capacity, size_t *read_size)
{
    char path[128];
    FILE *f;
    size_t got;
    if (read_size)
        *read_size = 0;
    if ((!data && capacity) || !MakePath(path, sizeof(path), name))
        return 0;
    f = fopen(path, "rb");
    if (!f)
        return 0;
    got = capacity ? fread(data, 1, capacity, f) : 0;
    if (ferror(f)) {
        fclose(f);
        return 0;
    }
    fclose(f);
    if (read_size)
        *read_size = got;
    return 1;
}
