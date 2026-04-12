# Embfetch

Embfetch is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for embedded, high-level abstraction machine such as JVM, and real-time operating systems such as Zephyr.

## Installation

Depending on which system you may have, the library may be installed in various embedded, RTOSes, and high-level language abstraction machines.
Currently, the supported systems are Zephyr RTOS and FreeRTOS.
### Zephyr RTOS
1. Make sure `CONFIG_SHELL` is `=y` to make sure the command can be used.
2. Additionally, to see the amount of heap pool, set `CONFIG_SYS_HEAP_RUNTIME_STATS` to `=y`, and to see the flash size, set `CONFIG_FLASH` to `=y`.
3. Copy `sysinfo_core.c` from `src`, `embed_sysinfo.h` from `src/include`, `sysinfo_zephyr.c` from `src/rtos/zephyr`, and `logo.h` from `src/rtos/zephyr/include` into your kernel source's directory.
4. Add 
```
static int cmd_embfetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    sysinfo_print_shell(sh);

    return 0;
}
```
And `SHELL_CMD_REGISTER(embfetch, NULL, "Show system information and logo", cmd_embfetch);` into your C source file responsible for making shell.
5. Add `src/sysinfo_core.c src/sysinfo_zephyr.c` into your kernel's `CMakeLists.txt`.
6. Run `west build -p always -b (your target board)` to clean up the previously built kernel and compile the library into the kernel.
7. After compilation, your kernel's shell will now have `embfetch` as available command.
### FreeRTOS
In FreeRTOS