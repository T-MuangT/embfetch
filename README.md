# Embfetch

Embfetch is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for real-time operating systems such as Zephyr and FreeRTOS.

## Installation

Depending on which system you may have, the library may be installed in various real-time operating systems.
Currently, the supported systems are Zephyr RTOS, RT-Thread, and FreeRTOS, both the vanilla and ESP-IDF versions.
### Zephyr RTOS
1. Make sure `CONFIG_SHELL` is `=y` to make sure the command can be used.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_zephyr.c` from `src/rtos/zephyr`, and `logo.h` from `src/rtos/zephyr/include` into your kernel `src` directory.
3. Add entry and register command into your C source file responsible for making shell.
4. Add `src/sysinfo_core.c src/sysinfo_zephyr.c` into `CMakeLists.txt`'s `target_sources()`.
5. Run `west build -b (your target board)` to compile the library into the kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### RT-Thread
1. Make sure FinSH is configured.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_rt-thread.c` from `src/rtos/rt-thread`, and `logo.h` from `src/rtos/rt-thread/include` into your kernel's `applications` directory.
3. If you use RT-Thread Studio, you can Rebuild Project. Otherwise, run `scons -j$(nproc)` to compile the library into the kernel.
4. After compilation, your kernel's shell will now have `embfetch` as available command.
### Vanilla FreeRTOS
1. Make sure FreeRTOS+CLI is set up.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_freertos.c` from `src/rtos/freertos`, and `logo.h` from `src/rtos/freertos/include` into your kernel's `src` directory.
3. Add entry and register command into your C source file responsible for making shell.
4. Add `sysinfo_core.c` and `sysinfo_freertos.c` into `SRCS` in Makefile.
5. Run `make` to compile the library into kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### ESP-IDF
1. Build the project with `system/console` template using Espressif IDE. Can be either basic or advanced.
2. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_esp-idf.c` from `src/rtos/esp-idf`, and `logo.h` from `src/rtos/esp-idf/include` into your kernel's `main` directory.
3. Add entry and register command into your C source file responsible for making shell, most likely name could be `console_main.c` or similar.
4. Add `sysinfo_core.c sysinfo_zephyr.c` into your `main/CMakeLists.txt`'s `SRCS`.
5. Add `console esp_timer heap esp_system esp_hw_support` into your `main/CMakeLists.txt`'s `REQUIRES`.
6. Rebuild Project.
7. After compilation, your kernel's shell will now have `embfetch` as available command.