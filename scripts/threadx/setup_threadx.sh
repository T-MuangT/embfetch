#!/bin/sh
# setup.sh
# Copies embfetch ThreadX port files into your project and updates CMakeLists.txt.
# Usage: ./setup.sh <destination_src_dir> [cmake_path]
#   destination_src_dir : path to your project's src directory
#   cmake_path          : path to your CMakeLists.txt (optional, defaults to <destination_src_dir>/../CMakeLists.txt)

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

if [ -z "$1" ]; then
    printf "Error: No destination directory specified.\n"
    printf "Usage: %s <destination_src_dir> [cmake_path].\n" "$0"
    exit 1
fi

DEST="$1"
CMAKE="${2:-$(dirname "$DEST")/CMakeLists.txt}"

if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '$DEST' does not exist.\n"
    exit 1
fi

if [ ! -f "$CMAKE" ]; then
    printf "Error: CMakeLists.txt not found at '$CMAKE'.\n"
    printf "Specify the CMakeLists.txt path as the second argument.\n"
    exit 1
fi

printf "Copying embfetch ThreadX port files...\n"

cp "$SRC/sysinfo_rtos_common.c"                "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/rtos_sysinfo.h"               "$DEST/rtos_sysinfo.h"
cp "$SRC/rtos/threadx/sysinfo_threadx.c"       "$DEST/sysinfo_threadx.c"
cp "$SRC/rtos/threadx/include/logo.h"          "$DEST/logo.h"

printf "Files copied to '$DEST':\n"
printf "  sysinfo_rtos_common.c\n"
printf "  rtos_sysinfo.h\n"
printf "  sysinfo_threadx.c\n"
printf "  logo.h\n"
printf "\n"
printf "Updating CMakeLists.txt at '$CMAKE'...\n"

if grep -q "sysinfo_rtos_common.c" "$CMAKE"; then
    printf "Warning: CMakeLists.txt already contains embfetch sources. Skipping edit.\n"
else
    if grep -q "add_executable" "$CMAKE"; then
        sed "/add_executable/,/)/{
            /)/i \\
            src/sysinfo_rtos_common.c \\
            src/sysinfo_threadx.c
        }" "$CMAKE" > "${CMAKE}.tmp" && mv "${CMAKE}.tmp" "$CMAKE"
        
        printf "CMakeLists.txt updated.\n"
    else
        printf "Warning: No add_executable() found in '$CMAKE'.\n"
        printf "Manually add the following to your add_executable():\n"
        printf "    src/sysinfo_rtos_common.c\n"
        printf "    src/sysinfo_threadx.c\n"
    fi
fi
printf "\n"
printf "Next steps:\n"
printf "  1. Add entry and register the embfetch command in your shell source file.\n"
printf "  2. Include 'rtos_sysinfo.h' in your shell source file.\n"