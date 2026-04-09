#include <stdio.h>
#include <src/include/sysinfo.h>
// This file provides common wrapper functions for sysinfo.
// The actual fetching may be done in system-specific sources such as sysinfo_zephyr.c or sysinfo_freertos.c.
static void file_putline(void *ctx, const char *line) {
    fprintf((FILE *)ctx, "%s\n", line);
}

void sysinfo_print_file(FILE *f) {
    sysinfo_print(file_putline, f);
}

static void puts_putline(void *ctx, const char *line) {
    ((void (*)(const char *))ctx)(line);
}

void sysinfo_print_puts(void (*puts_fn)(const char *)) {
    sysinfo_print(puts_putline, (void *)puts_fn);
}