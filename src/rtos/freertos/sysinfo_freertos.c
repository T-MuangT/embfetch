#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include <embed_sysinfo.h>
#include "logo.h"

// Static board info fetching.
static const sysinfo_static_t board_info = {
    .username       = "root",
    .hostname       = "stm32f411ceu6-disco",
    .os_name        = "FreeRTOS",
    .mcu            = "STM32F411CEU6",
    .build_date     = __DATE__ " " __TIME__,
};

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%uMB", (unsigned int)(bytes >> 20));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%uKB", (unsigned int)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB", (unsigned int)bytes);
    }
}

// Hardware info fetching. No devicetree in FreeRTOS, so sizes come from
// FreeRTOSConfig.h / your BSP. Define these there or pass via makefile.
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
#if defined(SYSINFO_RAM_SIZE)
    format_size(dst->ram, sizeof(dst->ram), SYSINFO_RAM_SIZE);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

#if defined(SYSINFO_FLASH_SIZE)
    format_size(dst->flash, sizeof(dst->flash), SYSINFO_FLASH_SIZE);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info fetching.
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Kernel version
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "FreeRTOS %s", tskKERNEL_VERSION_STRING);

    // Uptime from tick count
    // NOTE: TickType_t is uint32_t by default — wraps after ~49 days at 1000Hz.
    // For long-running uptime, maintain a separate overflow counter in a timer callback.
    TickType_t ticks   = xTaskGetTickCount();
    uint64_t   uptime_s = (uint64_t)ticks / configTICK_RATE_HZ;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // Heap stats
    // FreeRTOS only exposes free heap natively; used = total - free.
    // heap_4/heap_5 also provide xPortGetMinimumEverFreeHeapSize() for low-watermark.
    size_t heap_free  = xPortGetFreeHeapSize();
    size_t heap_total = configTOTAL_HEAP_SIZE;
    size_t heap_used  = (heap_total > heap_free) ? (heap_total - heap_free) : 0;

    format_size(dst->heap_used, sizeof(dst->heap_used), heap_used);
    format_size(dst->heap_free, sizeof(dst->heap_free), heap_free);
}

// Print logo and info.
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_t hw;
    sysinfo_hwinfo_fetch(&hw);

    char header[64];
    char separator[32];
    snprintf(header,    sizeof(header),    "%s@%s", board_info.username, board_info.hostname);
    snprintf(separator, sizeof(separator), "----------------");
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

    int logo_count = 0;
    while (logo[logo_count] != NULL) logo_count++;

    // Build formatted info lines to match logo
    char os_line[64],  kernel_line[64], mcu_line[64], build_line[64];
    char flash_line[64], ram_line[64];
    char uptime_line[64], heap_line[64];

    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", board_info.mcu);
    snprintf(ram_line,    sizeof(ram_line),    "Memory:  %s", hw.ram);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free", dyn.heap_used, dyn.heap_free);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", hw.flash);

    info[1]  = header;
    info[2]  = separator;
    info[3]  = os_line;
    info[4]  = kernel_line;
    info[5]  = uptime_line;
    info[6]  = build_line;
    info[7]  = mcu_line;
    info[8]  = ram_line;
    info[9]  = heap_line;
    info[10]  = flash_line;

    // Print side by side
    for (int i = 0; i < logo_count; i++) {
        const char *info_line = (info[i] != NULL) ? info[i] : "";
        char line[256];
        snprintf(line, sizeof(line), "%s   %s", logo[i], info_line);
        putline(ctx, line);
    }
}

// Wrapper for shell output.
static void uart_putline(void *ctx, const char *line) {
    (void)ctx;
    printf("%s\n", line);
}

void sysinfo_print_uart(void) {
    sysinfo_print(uart_putline, NULL);
}