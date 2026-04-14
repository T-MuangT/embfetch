# Embfetch

Embfetch is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for real-time operating systems such as Zephyr and FreeRTOS.

## Installation

Depending on which system you may have, the library may be installed in various embedded, RTOSes, and high-level language abstraction machines.

Currently, the supported systems are Zephyr RTOS and FreeRTOS.
### Zephyr RTOS
1. Make sure `CONFIG_SHELL` is `=y` to make sure the command can be used.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_zephyr.c` from `src/rtos/zephyr`, and `logo.h` from `src/rtos/zephyr/include` into your kernel source's directory.
3. Add entry and register command into your C source file responsible for making shell.
4. Add `src/sysinfo_core.c src/sysinfo_zephyr.c` into your kernel's `CMakeLists.txt`.
5. Run `west build -p always -b (your target board)` to clean up the previously built kernel and compile the library into the kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### FreeRTOS
1. Make sure FreeRTOS+CLI is set up.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_freertos.c` from `src/rtos/freertos`, and `logo.h` from `src/rtos/freertos/include` into your kernel source's directory.
3. Add entry and register command into your C source file responsible for making shell.
4. Add `sysinfo_core.c` and `sysinfo_freertos.c` into `SRCS` in Makefile.
5. Run `make` to compile the library into kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.

