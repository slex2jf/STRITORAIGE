/*
 * annotations.c — CSV annotation loader.
 *
 * Reads a <game>_annotations.csv file that associates ROM addresses with
 * human-readable names and notes. Used by code_generator.c to emit
 * comment headers above each recompiled function.
 *
 * CSV format (no header row):
 *   <hex_addr>,<name>,<notes>
 *   000200,Reset_Handler,"Entry point from vector table"
 *   001234,SomeFunc,"Does XYZ"
 *
 * Lines beginning with '#' are comments and are ignored.
 */
#include "annotations.h"
#include "portable_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copy_string(char *dst, size_t dst_size, const char *src) {
    if (dst_size == 0) return;
    size_t len = strlen(src);
    if (len >= dst_size) len = dst_size - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
}

bool annotations_load(AnnotationTable *out, const char *path) {
    FILE *f = m68k_fopen(path, "r");
    if (!f) return false;

    char line[512];
    int capacity = 64;
    out->entries = (Annotation *)malloc(capacity * sizeof(Annotation));
    if (!out->entries) { fclose(f); return false; }
    out->count = 0;

    while (fgets(line, sizeof(line), f)) {
        /* Strip trailing newline */
        int len = (int)strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (len == 0 || line[0] == '#') continue;

        /* Parse: addr,name,notes */
        char addr_str[16] = {0}, name[128] = {0}, notes[256] = {0};
        char *name_tok = strchr(line, ',');
        char *notes_tok = NULL;
        if (name_tok) {
            *name_tok++ = '\0';
            notes_tok = strchr(name_tok, ',');
            if (notes_tok) *notes_tok++ = '\0';
        }

        copy_string(addr_str, sizeof(addr_str), line);
        if (name_tok) copy_string(name, sizeof(name), name_tok);

        if (notes_tok) {
            /* Strip surrounding quotes if present */
            if (notes_tok[0] == '"') notes_tok++;
            size_t nlen = strlen(notes_tok);
            if (nlen > 0 && notes_tok[nlen-1] == '"') notes_tok[nlen-1] = '\0';
            copy_string(notes, sizeof(notes), notes_tok);
        }

        if (out->count >= capacity) {
            capacity *= 2;
            Annotation *tmp = realloc(out->entries, capacity * sizeof(Annotation));
            if (!tmp) break;
            out->entries = tmp;
        }

        Annotation *a = &out->entries[out->count++];
        a->addr = (uint32_t)strtoul(addr_str, NULL, 16);
        copy_string(a->name, sizeof(a->name), name);
        copy_string(a->notes, sizeof(a->notes), notes);
    }

    fclose(f);
    return out->count > 0;
}

const char *annotations_get_name(const AnnotationTable *at, uint32_t addr) {
    for (int i = 0; i < at->count; i++)
        if (at->entries[i].addr == addr)
            return at->entries[i].name[0] ? at->entries[i].name : NULL;
    return NULL;
}

const char *annotations_get_notes(const AnnotationTable *at, uint32_t addr) {
    for (int i = 0; i < at->count; i++)
        if (at->entries[i].addr == addr)
            return at->entries[i].notes[0] ? at->entries[i].notes : NULL;
    return NULL;
}

void annotations_free(AnnotationTable *at) {
    free(at->entries);
    at->entries = NULL;
    at->count   = 0;
}
