#!/bin/sh
# package_rtthread_manual.sh
# Copies embfetch RT-Thread port files into applications/ and updates SConscript.
# Usage: ./package_rtthread_manual.sh <applications_dir> [sconscript_path]
#   applications_dir : path to your RT-Thread kernel's applications/ directory
#   sconscript_path  : path to your SConscript (optional, defaults to <applications_dir>/SConscript)

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

if [ -z "$1" ]; then
    printf "Error: No applications/ directory specified.\n"
    printf "Usage: %s <applications_dir> [sconscript_path]\n" "$0"
    exit 1
fi

APPS_DIR="$1"
SCONSCRIPT="${2:-$APPS_DIR/SConscript}"

if [ ! -d "$APPS_DIR" ]; then
    printf "Error: applications/ directory '%s' does not exist.\n" "$APPS_DIR"
    exit 1
fi

if [ ! -f "$SCONSCRIPT" ]; then
    printf "Error: SConscript not found at '%s'.\n" "$SCONSCRIPT"
    printf "Specify the SConscript path as the second argument.\n"
    exit 1
fi

printf "Copying embfetch RT-Thread port files...\n"

cp "$SRC/sysinfo_rtos_common.c"                    "$APPS_DIR/sysinfo_rtos_common.c"
cp "$SRC/include/embed_sysinfo.h"                  "$APPS_DIR/embed_sysinfo.h"
cp "$SRC/rtos/rt-thread/sysinfo_rt-thread.c"       "$APPS_DIR/sysinfo_rt-thread.c"
cp "$SRC/rtos/rt-thread/include/logo.h"            "$APPS_DIR/logo.h"

printf "Files copied to '%s':\n" "$APPS_DIR"
printf "  sysinfo_rtos_common.c\n"
printf "  embed_sysinfo.h\n"
printf "  sysinfo_rt-thread.c\n"
printf "  logo.h\n"

printf "\n"
printf "Updating SConscript at '%s'...\n" "$SCONSCRIPT"

if grep -q "sysinfo_rtos_common.c" "$SCONSCRIPT"; then
    printf "Warning: SConscript already contains embfetch sources. Skipping edit.\n"
else
    if grep -q "src" "$SCONSCRIPT"; then
        sed "/src = \[/,/\]/{
            /\]/i \\
    'sysinfo_rtos_common.c',\\
    'sysinfo_rt-thread.c',
        }" "$SCONSCRIPT" > "${SCONSCRIPT}.tmp" && mv "${SCONSCRIPT}.tmp" "$SCONSCRIPT"
        printf "SConscript updated.\n"
    else
        printf "Warning: No src list found in SConscript.\n"
        printf "Manually add the following to your src list:\n"
        printf "  'sysinfo_rtos_common.c',\n"
        printf "  'sysinfo_rt-thread.c',\n"
    fi
fi

printf "\n"
printf "Next steps:\n"
printf "  1. Make sure FinSH is configured in your RT-Thread kernel.\n"
printf "  2. Add entry and register the embfetch command in your shell source file.\n"
printf "  3. Include 'embed_sysinfo.h' in your shell source file.\n"