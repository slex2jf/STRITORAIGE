#ifndef M68K_RECOMP_PORTABLE_IO_H
#define M68K_RECOMP_PORTABLE_IO_H

#include <stdio.h>

static inline FILE *m68k_fopen(const char *path, const char *mode) {
#if defined(_MSC_VER)
    FILE *file = NULL;
    return fopen_s(&file, path, mode) == 0 ? file : NULL;
#else
    return fopen(path, mode);
#endif
}

#endif
