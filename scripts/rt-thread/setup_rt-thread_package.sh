#!/bin/sh
# package_rtthread_package.sh
# Enables embfetch in RT-Thread's .config for package manager installation.
# Usage: ./package_rtthread_package.sh <rtthread_root>
#   rtthread_root : path to your RT-Thread kernel root directory

set -e

if [ -z "$1" ]; then
    printf "Error: No RT-Thread root directory specified.\n"
    printf "Usage: %s <rtthread_root>\n" "$0"
    exit 1
fi

RTTHREAD_ROOT="$1"
CONFIG="$RTTHREAD_ROOT/.config"

if [ ! -d "$RTTHREAD_ROOT" ]; then
    printf "Error: RT-Thread root '%s' does not exist.\n" "$RTTHREAD_ROOT"
    exit 1
fi

if [ ! -f "$CONFIG" ]; then
    printf "Error: .config not found at '%s'.\n" "$CONFIG"
    printf "Make sure you have run menuconfig at least once to generate .config.\n"
    exit 1
fi

printf "Enabling PKG_USING_EMBFETCH in '%s'...\n" "$CONFIG"

if grep -q "PKG_USING_EMBFETCH" "$CONFIG"; then
    if grep -q "# PKG_USING_EMBFETCH is not set" "$CONFIG"; then
        sed "s/# PKG_USING_EMBFETCH is not set/PKG_USING_EMBFETCH=y/" \
            "$CONFIG" > "${CONFIG}.tmp" && mv "${CONFIG}.tmp" "$CONFIG"
        printf ".config updated: PKG_USING_EMBFETCH enabled.\n"
    else
        printf "Warning: PKG_USING_EMBFETCH already enabled. Skipping.\n"
    fi
else
    printf "\n" >> "$CONFIG"
    printf "# embfetch\n" >> "$CONFIG"
    printf "PKG_USING_EMBFETCH=y\n" >> "$CONFIG"
    printf ".config updated: PKG_USING_EMBFETCH added and enabled.\n"
fi

printf "\n"
printf "Next steps:\n"
printf "  1. Run 'pkgs --update' to fetch the embfetch package.\n"
printf "  2. Make sure FinSH is configured in your RT-Thread kernel.\n"