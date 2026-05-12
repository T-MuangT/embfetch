#!/bin/sh
# package_espidf.sh
# Copies embfetch ESP-IDF port files into your project and updates main/CMakeLists.txt.
# Usage: ./package_espidf.sh <destination_main_dir> [cmake_path]
#   destination_main_dir : path to your project's main directory
#   cmake_path           : path to your main/CMakeLists.txt (optional, defaults to <destination_main_dir>/CMakeLists.txt)

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

if [ -z "$1" ]; then
    printf "Error: No destination directory specified.\n"
    printf "Usage: %s <destination_main_dir> [cmake_path]\n" "$0"
    exit 1
fi

DEST="$1"
CMAKE="${2:-$DEST/CMakeLists.txt}"

if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '$DEST' does not exist.\n"
    exit 1
fi

if [ ! -f "$CMAKE" ]; then
    printf "Error: CMakeLists.txt not found at '$CMAKE'.\n"
    printf "Specify the CMakeLists.txt path as the second argument.\n"
    exit 1
fi

printf "Copying embfetch ESP-IDF port files...\n"

cp "$SRC/sysinfo_rtos_common.c"                         "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/rtos_sysinfo.h"                        "$DEST/rtos_sysinfo.h"
cp "$SRC/rtos/freertos/esp-idf/sysinfo_esp-idf.c"       "$DEST/sysinfo_esp-idf.c"
cp "$SRC/rtos/freertos/esp-idf/include/logo.h"          "$DEST/logo.h"

printf "Files copied to '$DEST':\n"
printf "  sysinfo_rtos_common.c\n"
printf "  rtos_sysinfo.h\n"
printf "  sysinfo_esp-idf.c\n"
printf "  logo.h\n"

printf "\n"
printf "Updating SRCS in '$CMAKE'...\n"

if grep -q "sysinfo_rtos_common.c" "$CMAKE"; then
    printf "Warning: CMakeLists.txt already contains embfetch sources. Skipping SRCS edit.\n"
else
    if grep -q "SRCS" "$CMAKE"; then
        sed "/SRCS/a\\
        sysinfo_rtos_common.c\\
        sysinfo_esp-idf.c" "$CMAKE" > "${CMAKE}.tmp" 
        && mv "${CMAKE}.tmp" "$CMAKE"
    else
        printf "\n" >> "$CMAKE"
        printf "# embfetch" >> "$CMAKE"
        printf "set(SRCS \${SRCS} sysinfo_rtos_common.c sysinfo_esp-idf.c)" >> "$CMAKE"
    fi
    printf "SRCS updated.\n"
fi

printf "\n"
printf "Updating REQUIRES in '$CMAKE'..."

if grep -q "esp_timer" "$CMAKE"; then
    printf "Warning: CMakeLists.txt already contains embfetch REQUIRES. Skipping REQUIRES edit.\n"
else
    if grep -q "REQUIRES" "$CMAKE"; then
        sed "/REQUIRES/a\\    
        console\\n
        esp_timer\\n
        heap\\n
        esp_system\\n
        esp_hw_support" "$CMAKE" > "${CMAKE}.tmp" 
        && mv "${CMAKE}.tmp" "$CMAKE"
    else
        printf "\n" >> "$CMAKE"
        printf "# embfetch requirements" >> "$CMAKE"
        printf "set(REQUIRES \${REQUIRES} console esp_timer heap esp_system esp_hw_support)" >> "$CMAKE"
    fi
    printf "REQUIRES updated.\n"
fi

printf "\n"
printf "Next steps:\n"
printf "  1. Add entry and register the embfetch command in your main or shell source file.\n"
printf "  2. Include 'rtos_sysinfo.h' in your shell source file.\n"
