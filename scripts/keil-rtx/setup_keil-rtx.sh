#!/bin/sh
# package_keilrtx.sh
# Copies embfetch Keil RTX port files into your project.
# Usage: ./package_keilrtx.sh <destination_src_dir>
#   destination_src_dir : path to your project's src directory

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

if [ -z "$1" ]; then
    printf "Error: No destination directory specified.\n"
    printf "Usage: %s <destination_src_dir>\n" "$0"
    exit 1
fi

DEST="$1"

if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '$DEST' does not exist.\n"
    exit 1
fi

printf "Copying embfetch Keil RTX port files...\n"

cp "$SRC/sysinfo_rtos_common.c"                 "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/embed_sysinfo.h"               "$DEST/embed_sysinfo.h"
cp "$SRC/rtos/keil-rtx/sysinfo_keilrtx.c"       "$DEST/sysinfo_keilrtx.c"
cp "$SRC/rtos/keil-rtx/include/logo.h"          "$DEST/logo.h"

printf "Files copied to '$DEST':\n"
printf "  sysinfo_rtos_common.c\n"
printf "  embed_sysinfo.h\n"
printf "  sysinfo_keilrtx.c\n"
printf "  logo.h\n"

printf "\n"
printf "Next steps:\n"
printf "  1. Add the copied files to your project using one of the following:\n"
printf "     - uVision: Project > Add Existing Files to Group\n"
printf "     - MDK v6 / VS Code: Add files to your project's CMakeLists.txt manually\n"
printf "  2. Add entry and register the embfetch command in your shell source file.\n"
printf "  3. Include 'embed_sysinfo.h' in your shell source file.\n"