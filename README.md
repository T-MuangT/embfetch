# Embfetch RTOS

Embfetch RTOS is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for real-time operating systems such as Zephyr and FreeRTOS.

## Installation

Depending on which system you may have, the library may be installed in various real-time operating systems.
Currently, the supported systems are Zephyr, RT-Thread, NuttX, ThreadX, FreeRTOS, ESP-IDF, and Keil RTX5.
### Zephyr
1. Make sure `CONFIG_SHELL` is `=y` to make sure the command can be used.
2. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_zephyr.c` from `src/rtos/zephyr`, and `logo.h` from `src/rtos/zephyr/include` into your kernel `src` directory.
3. Add entry and register `embfetch` command into your main or shell source file.
4. Add `src/sysinfo_rtos_common.c src/sysinfo_zephyr.c` into `CMakeLists.txt`'s `target_sources()`.
5. Run `west build -b (your target board)` to compile the library into the kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### RT-Thread
1. Make sure FinSH is configured.
2. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_rt-thread.c` from `src/rtos/rt-thread`, and `logo.h` from `src/rtos/rt-thread/include` into your kernel's `applications` directory.
3. If you use RT-Thread Studio, you can Rebuild Project. Otherwise, run `scons -j$(nproc)` to compile the library into the kernel.
4. After compilation, your kernel's shell will now have `embfetch` as available command.
### NuttX (Apache NuttX)
1. Due to NuttX's native `nsh`, the library can be compiled as application.
2. Make a new `embfetch` directory for the library in `apps/examples`.
3. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `embfetch_main.c`, `sysinfo_nuttx.c`, `Kconfig`, `Makefile`, `Make.defs` from `src/rtos/nuttx`, and `logo.h` from `src/rtos/nuttx/include` into `apps/examples/embfetch`.
4. Add `source "examples/embfetch/Kconfig"` into `apps/examples/Kconfig` file.
5. Run `make` to compile the library as application for the kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### ThreadX (Eclipse ThreadX, formerly Azure RTOS)
1. ThreadX has no built-in shell option, nor the official shell library. Make sure the shell library (vendor, or custom) is set up.
2. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_threadx.c` from `src/rtos/threadx`, and `logo.h` from `src/rtos/threadx/include` into your kernel's `src` directory.
3. Add entry and register `embfetch` command into your main or shell source file.
4. Add `src/sysinfo_rtos_common.c src/sysinfo_threadx.c` into `CMakeLists.txt`'s `add_executable()`.
5. Run `cmake -B build` and `cmake --build build` to compile the library into the kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### FreeRTOS
1. FreeRTOS has no built-in shell option. However, FreeRTOS has FreeRTOS + CLI as its official shell library. Make sure shell library (FreeRTOS + CLI, vendor, or custom) is set up.
2. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_freertos.c` from `src/rtos/freertos`, and `logo.h` from `src/rtos/freertos/include` into your kernel's `src` directory.
3. Add entry and register `embfetch` command into your main or shell source file.
4. Add `sysinfo_rtos_common.c` and `sysinfo_freertos.c` into `SRCS` in Makefile.
5. Run `make` to compile the library into kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.
### ESP-IDF
1. Build the ESP-IDF project with `system/console` template using Espressif IDE. Can be either basic or advanced.
2. Copy `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_esp-idf.c` from `src/rtos/esp-idf`, and `logo.h` from `src/rtos/esp-idf/include` into your kernel's `main` directory.
3. Add entry and register `embfetch` command into your main or shell source file.
4. Add `sysinfo_rtos_common.c sysinfo_esp-idf.c` into your `main/CMakeLists.txt`'s `SRCS`.
5. Add `console esp_timer heap esp_system esp_hw_support` into your `main/CMakeLists.txt`'s `REQUIRES`.
6. Rebuild Project to compile the library into kernel.
7. After compilation, your kernel's shell will now have `embfetch` as available command.
### Keil RTX5
1. Build the Keil RTX5 project with `CMSIS/Core`, `CMSIS/OS tick/SysTick`, and `CMSIS/RTOS2/Keil RTX5` in runtime environment selected.
2. Keil RTX5 has no built-in shell option, nor the official shell library. Make sure the shell library (vendor, or custom) is set up, along with adding `USING_KEIL_RTX` into target's defined preprocessor symbols.
3. Add `sysinfo_rtos_common.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_keil-rtx.c` from `src/rtos/keil-rtx`, and `logo.h` from `src/rtos/keil-rtx/include` into your Keil RTX5 project.
4. Add entry and register `embfetch` command into your main or shell source file.
5. Rebuild Project to compile the library into kernel.
6. After compilation, your kernel's shell will now have `embfetch` as available command.