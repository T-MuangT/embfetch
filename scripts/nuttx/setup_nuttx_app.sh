#!/bin/sh
# package_nuttx.sh
# Copies embfetch NuttX port files into apps/examples/embfetch and updates apps/examples/Kconfig.
# Usage: ./package_nuttx.sh <apps_dir>
#   apps_dir : path to your NuttX apps/ directory

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$EMBFETCH_ROOT/src"

# --- Argument handling ---
if [ -z "$1" ]; then
    printf "Error: No apps/ directory specified.\n"
    printf "Usage: %s <apps_dir>\n" "$0"
    exit 1
fi

APPS_DIR="$1"
EXAMPLES_DIR="$APPS_DIR/examples"
DEST="$EXAMPLES_DIR/embfetch"
KCONFIG="$EXAMPLES_DIR/Kconfig"

# --- Validate paths ---
if [ ! -d "$APPS_DIR" ]; then
    printf "Error: apps/ directory '%s' does not exist.\n" "$APPS_DIR"
    exit 1
fi

if [ ! -d "$EXAMPLES_DIR" ]; then
    printf "Error: examples/ directory '%s' does not exist.\n" "$EXAMPLES_DIR"
    exit 1
fi

if [ ! -f "$KCONFIG" ]; then
    printf "Error: Kconfig not found at '%s'.\n" "$KCONFIG"
    exit 1
fi

# --- Create embfetch app directory ---
if [ -d "$DEST" ]; then
    printf "Warning: '%s' already exists. Files may be overwritten.\n" "$DEST"
else
    mkdir -p "$DEST"
    printf "Created '%s'.\n" "$DEST"
fi

# --- Copy files ---
printf "Copying embfetch NuttX port files...\n"

cp "$SRC/sysinfo_rtos_common.c"              "$DEST/sysinfo_rtos_common.c"
cp "$SRC/include/embed_sysinfo.h"            "$DEST/embed_sysinfo.h"
cp "$SRC/rtos/nuttx/embfetch_main.c"        "$DEST/embfetch_main.c"
cp "$SRC/rtos/nuttx/sysinfo_nuttx.c"        "$DEST/sysinfo_nuttx.c"
cp "$SRC/rtos/nuttx/Kconfig"                "$DEST/Kconfig"
cp "$SRC/rtos/nuttx/Makefile"               "$DEST/Makefile"
cp "$SRC/rtos/nuttx/Make.defs"              "$DEST/Make.defs"
cp "$SRC/rtos/nuttx/include/logo.h"         "$DEST/logo.h"

printf "Files copied to '%s':\n" "$DEST"
printf "  sysinfo_rtos_common.c\n"
printf "  embed_sysinfo.h\n"
printf "  embfetch_main.c\n"
printf "  sysinfo_nuttx.c\n"
printf "  Kconfig\n"
printf "  Makefile\n"
printf "  Make.defs\n"
printf "  logo.h\n"

# --- Edit apps/examples/Kconfig ---
printf "\n"
printf "Updating '%s'...\n" "$KCONFIG"

if grep -q "examples/embfetch/Kconfig" "$KCONFIG"; then
    printf "Warning: Kconfig already contains embfetch entry. Skipping.\n"
else
    printf "\n" >> "$KCONFIG"
    printf "source \"examples/embfetch/Kconfig\"\n" >> "$KCONFIG"
    printf "Kconfig updated.\n"
fi

# --- Next steps ---
printf "\n"
printf "Next steps:\n"
printf "  1. Run 'make menuconfig' and enable embfetch under Application Configuration > Examples.\n"
printf "  2. Run 'make' to compile embfetch into the kernel.\n"
printf "  3. After compilation, 'embfetch' will be available as an NSH command.\n"