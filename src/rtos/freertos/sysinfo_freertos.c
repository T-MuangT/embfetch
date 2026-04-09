#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/sys_heap.h>
#include <zephyr/shell/shell.h>
#include "../include/sysinfo.h"
#include "logo.h"

/* --------------------------------------------------------------------------
 * Static board info — filled from Zephyr's devicetree/Kconfig
 * -------------------------------------------------------------------------- */
static const sysinfo_static_t board_info = {
    .username       = "root",
    .hostname       = CONFIG_BOARD,
    .os_name        = "Zephyr RTOS",
    .mcu            = CONFIG_SOC,
    .flash          = "2MB",   // hardcoded for RP2040, could be fetched from Kconfig if needed
    .ram            = "264KB"    // hardcoded for RP2040, could be fetched from Kconfig if needed
};

/* --------------------------------------------------------------------------
 * Helper — formats raw bytes into a human readable string
 * -------------------------------------------------------------------------- */
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%uMB", (unsigned int)(bytes >> 20));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%uKB", (unsigned int)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB", (unsigned int)bytes);
    }
}

/* --------------------------------------------------------------------------
 * Fetch dynamic runtime data
 * -------------------------------------------------------------------------- */
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Uptime
    uint32_t ver = sys_kernel_version_get();
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "Zephyr %d.%d.%d",
             SYS_KERNEL_VER_MAJOR(ver),
             SYS_KERNEL_VER_MINOR(ver),
             SYS_KERNEL_VER_PATCHLEVEL(ver));
    int64_t uptime_ms = k_uptime_get();
    int64_t uptime_s  = uptime_ms / 1000;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // Heap
    struct sys_memory_stats stats;
    #if defined(CONFIG_SYS_HEAP_RUNTIME_STATS)
        extern struct sys_heap _system_heap;
        sys_heap_runtime_stats_get(&_system_heap, &stats);
    #else
        stats.allocated_bytes = 0;
        stats.free_bytes = 0;
    #endif
    format_size(dst->heap_used, sizeof(dst->heap_used), stats.allocated_bytes);
    format_size(dst->heap_free, sizeof(dst->heap_free), stats.free_bytes);
}

/* --------------------------------------------------------------------------
 * Print logo + info
 * -------------------------------------------------------------------------- */
void sysinfo_print(const struct shell *sh) {
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);

    char header[64];
    char separator[32];
    snprintf(header,    sizeof(header),    "%s@%s", board_info.username, board_info.hostname);
    snprintf(separator, sizeof(separator), "----------------");

    // Info lines — same count as logo lines
    const char *info[] = {
        "",
        header,
        separator,
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        NULL
    };

    // Count logo lines
    int logo_count = 0;
    while (logo[logo_count] != NULL) logo_count++;

    // Build formatted info lines to match logo
    char os_line[64],  kernel_line[64], mcu_line[64];
    char flash_line[64], ram_line[64];
    char uptime_line[64], heap_line[64];

    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", board_info.mcu);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", board_info.flash);
    snprintf(ram_line,    sizeof(ram_line),    "RAM:     %s", board_info.ram);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free", dyn.heap_used, dyn.heap_free);

    // Assign to info slots — adjust indices to taste
    info[1]  = header;
    info[2]  = separator;
    info[3]  = os_line;
    info[4]  = kernel_line;
    info[5]  = mcu_line;
    info[6]  = flash_line;
    info[7]  = ram_line;
    info[8]  = uptime_line;
    info[9]  = heap_line;

    // Print side by side
    for (int i = 0; i < logo_count; i++) {
        const char *info_line = (info[i] != NULL) ? info[i] : "";
        shell_print(sh, "%s   %s", logo[i], info_line);
    }
}