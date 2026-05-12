#!/bin/sh
# setup.sh
# Copies embfetch FreeRTOS port files into your project and updates Makefile SRCS.
# Usage: ./setup.sh <destination_src_dir> [makefile_path]
#   destination_src_dir : path to your project's src directory
#   makefile_path       : path to your Makefile (optional, defaults to <destination_src_dir>/../Makefile)
 
set -e
 
EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"
 
if [ -z "$1" ]; then
    printf "Error: No destination directory specified.\n"
    printf "Usage: %s <destination_src_dir> [makefile_path].\n" "$0"
    exit 1
fi
 
DEST="$1"
MAKEFILE="${2:-$(dirname "$DEST")/Makefile}"
 
if [ ! -d "$DEST" ]; then
    printf "Error: Destination directory '$DEST' does not exist.\n"
    exit 1
fi
 
if [ ! -f "$MAKEFILE" ]; then
    printf "Error: Makefile not found at '$MAKEFILE'.\n"
    printf "Specify the Makefile path as the second argument.\n"
    exit 1
fi
 
printf "Copying embfetch FreeRTOS port files...\n"
 
cp "$SRC/sysinfo_rtos_common.c"                         "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/rtos_sysinfo.h"                        "$DEST/rtos_sysinfo.h"
cp "$SRC/rtos/freertos/freertos/sysinfo_freertos.c"     "$DEST/sysinfo_freertos.c"
cp "$SRC/rtos/freertos/freertos/include/logo.h"         "$DEST/logo.h"
 
printf "Files copied to '$DEST':\n"
printf "  sysinfo_rtos_common.c\n"
printf "  rtos_sysinfo.h\n"
printf "  sysinfo_freertos.c\n"
printf "  logo.h\n"
printf "\n"
printf "Updating Makefile SRCS in '$MAKEFILE'..."
 
if grep -q "sysinfo_rtos_common.c" "$MAKEFILE"; then
    printf "Warning: Makefile already contains embfetch sources. Skipping SRCS edit.\n"
else
    TMP_FILE="${MAKEFILE}.tmp"
    if grep -q "^SRCS" "$MAKEFILE"; then
        sed "s/^SRCS[[:space:]]*=/SRCS +=/" "$MAKEFILE" > "$TMP_FILE"
        mv "$TMP_FILE" "$MAKEFILE"
        printf "\n" >> "$MAKEFILE"
        printf "# embfetch\n" >> "$MAKEFILE"
        printf "SRCS += sysinfo_rtos_common.c sysinfo_freertos.c\n" >> "$MAKEFILE"
    else
        printf "\n" >> "$MAKEFILE"
        printf "# embfetch\n" >> "$MAKEFILE"
        printf "SRCS += sysinfo_rtos_common.c sysinfo_freertos.c\n" >> "$MAKEFILE"
    fi
    printf "Makefile updated.\n"
fi
printf "\n"
printf "Next steps:\n"
printf "  1. Add entry and register embfetch command in your shell source file.\n"
printf "  2. Include 'rtos_sysinfo.h' in your shell source file.\n"