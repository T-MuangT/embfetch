#!/bin/sh
# setup_zephyr_builtin.sh
# Copies embfetch Zephyr port files into your project and updates CMakeLists.txt target_sources().
# Usage: ./setup_zephyr_builtin.sh <destination_src_dir> [cmake_path]
#   destination_src_dir : path to your project's src directory
#   cmake_path          : path to your CMakeLists.txt (optional, defaults to <destination_src_dir>/../CMakeLists.txt)

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

if [ -z "$1" ]; then
    printf "Error: No destination directory specified.\n"
    printf "Usage: %s <destination_src_dir> [cmake_path]\n" "$0"
    exit 1
fi

DEST="$1"
CMAKE="${2:-$(dirname "$DEST")/CMakeLists.txt}"

if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '%s' does not exist.\n" "$DEST"
    exit 1
fi

if [ ! -f "$CMAKE" ]; then
    printf "Error: CMakeLists.txt not found at '%s'.\n" "$CMAKE"
    printf "Specify the CMakeLists.txt path as the second argument.\n"
    exit 1
fi

printf "Copying embfetch Zephyr port files...\n"

cp "$SRC/sysinfo_rtos_common.c"                     "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/rtos_sysinfo.h"                    "$DEST/rtos_sysinfo.h"
cp "$SRC/rtos/zephyr/sysinfo_zephyr_common.c"       "$DEST/sysinfo_zephyr_common.c"
cp "$SRC/rtos/zephyr/zephyr/sysinfo_zephyr.c"       "$DEST/sysinfo_zephyr.c"
cp "$SRC/rtos/zephyr/zephyr/include/logo.h"         "$DEST/logo.h"

printf "Files copied to '%s':\n" "$DEST"
printf "  sysinfo_rtos_common.c\n"
printf "  rtos_sysinfo.h\n"
printf "  sysinfo_zephyr_common.c\n"
printf "  sysinfo_zephyr.c\n"
printf "  logo.h\n"

printf "\n"
printf "Updating target_sources() in '%s'...\n" "$CMAKE"

if grep -q "sysinfo_rtos_common.c" "$CMAKE"; then
    printf "Warning: CMakeLists.txt already contains embfetch sources. Skipping edit.\n"
else
    if grep -q "target_sources" "$CMAKE"; then
        sed "/target_sources/,/)/{
            /)/i \\
    src/sysinfo_rtos_common.c \\
    src/sysinfo_zephyr_common.c \\
    src/sysinfo_zephyr.c
        }" "$CMAKE" > "${CMAKE}.tmp" && mv "${CMAKE}.tmp" "$CMAKE"
        printf "CMakeLists.txt updated.\n"
    else
        printf "Warning: No target_sources() found in '%s'.\n" "$CMAKE"
        printf "Manually add the following to your target_sources():\n"
        printf "    src/sysinfo_rtos_common.c\n"
        printf "    src/sysinfo_zephyr_common.c\n"
        printf "    src/sysinfo_zephyr.c\n"
    fi
fi

printf "\n"
printf "Next steps:\n"
printf "  1. Make sure CONFIG_SHELL=y is set in your prj.conf.\n"
printf "  2. Add entry and register the embfetch command in your main or shell source file.\n"
printf "  3. Include 'rtos_sysinfo.h' in your shell source file.\n"