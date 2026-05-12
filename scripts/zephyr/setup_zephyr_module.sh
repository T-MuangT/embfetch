#!/bin/sh
# setup_zephyr_module.sh
# Adds embfetch as a west module to your Zephyr project's west.yml and copies prj.conf.example.
# Usage: ./setup_zephyr_module.sh <project_root> [west_yml_path]
#   project_root   : path to your Zephyr project root
#   west_yml_path  : path to your west.yml (optional, defaults to <project_root>/west.yml)

set -e

EMBFETCH_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"

# --- Argument handling ---
if [ -z "$1" ]; then
    printf "Error: No project root specified.\n"
    printf "Usage: %s <project_root> [west_yml_path]\n" "$0"
    exit 1
fi

PROJECT_ROOT="$1"
WEST_YML="${2:-$PROJECT_ROOT/west.yml}"

# --- Validate paths ---
if [ ! -d "$PROJECT_ROOT" ]; then
    printf "Error: Project root '%s' does not exist.\n" "$PROJECT_ROOT"
    exit 1
fi

if [ ! -f "$WEST_YML" ]; then
    printf "Error: west.yml not found at '%s'.\n" "$WEST_YML"
    printf "Specify the west.yml path as the second argument.\n"
    exit 1
fi

# --- Check for existing embfetch entry ---
if grep -q "embfetch" "$WEST_YML"; then
    printf "Warning: west.yml already contains an embfetch entry. Skipping.\n"
else
    # Append embfetch project entry under projects section
    printf "\n" >> "$WEST_YML"
    printf "    # embfetch\n" >> "$WEST_YML"
    printf "    - name: embfetch-rtos\n" >> "$WEST_YML"
    printf "      url: https://github.com/yourname/embfetch-rtos\n" >> "$WEST_YML"
    printf "      revision: main\n" >> "$WEST_YML"
    printf "      path: modules/embfetch-rtos\n" >> "$WEST_YML"
    printf "west.yml updated.\n"
fi

# --- Copy prj.conf.example ---
printf "\n"
printf "Copying prj.conf.example to '%s'...\n" "$PROJECT_ROOT"

cp "$EMBFETCH_ROOT/src/rtos/zephyr/vanilla/prj.conf.example" \
   "$PROJECT_ROOT/prj_embfetch.conf"

printf "Copied as 'prj_embfetch.conf'.\n"

# --- Next steps ---
printf "\n"
printf "Next steps:\n"
printf "  1. Update the 'url' in west.yml to point to your actual embfetch-rtos repository.\n"
printf "  2. Review 'prj_embfetch.conf' and merge the needed configs into your 'prj.conf':\n"
printf "       CONFIG_SHELL=y\n"
printf "       CONFIG_SHELL_STACK_SIZE=(your_value)\n"
printf "       CONFIG_SYS_HEAP_RUNTIME_STATS=y\n"
printf "  3. Run 'west update' to fetch the embfetch-rtos module.\n"
printf "  4. Add entry and register the embfetch command in your main or shell source file.\n"
printf "  5. Run 'west build -b <your_target_board>' to build.\n"