#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "embed_sysinfo.h"
#include "logo.h"
#include "uart.h"
//#include "board.h"      // define BOARD_NAME and MCU_NAME in your board header then uncomment

// Static board info fetching
static const sysinfo_static_t board_info = {
    .username       = "root",
    .os_name        = "FreeRTOS",
    .build_date     = __DATE__ " " __TIME__,
#if defined(BOARD_NAME) && defined(MCU_NAME)
    .hostname = BOARD_NAME,
    .mcu      = MCU_NAME,
#else
    .hostname = "Unknown",
    .mcu      = "Unknown",
#endif
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

// Hardware info fetching
// No devicetree in FreeRTOS, so sizes come from FreeRTOSConfig.h or your BSP
// Define these there or pass via makefile
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
             "FreeRTOS %d.%d.%d", tskKERNEL_VERSION_MAJOR, tskKERNEL_VERSION_MINOR, tskKERNEL_VERSION_BUILD);

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

    // All data lines
    char header[64], separator[32];
    char os_line[64], kernel_line[64], mcu_line[64], build_line[64], 
         flash_line[64], ram_line[64], uptime_line[64], heap_line[64];

    snprintf(header,      sizeof(header), "%s@%s", board_info.username, board_info.hostname);
    snprintf(separator,   sizeof(separator), "----------------");
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %luh %lum %lus", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", board_info.mcu);
    snprintf(ram_line,    sizeof(ram_line),    "Memory:  %s", hw.ram);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free", dyn.heap_used, dyn.heap_free);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", hw.flash);

    const char *info[] = {
        header, separator, os_line, kernel_line, 
        uptime_line, build_line, mcu_line, ram_line, 
        heap_line, flash_line
    };

    int logo_count = 0;
    while (logo[logo_count] != NULL) logo_count++;
    int info_count = sizeof(info) / sizeof(info[0]);
    int total_lines = (logo_count > info_count) ? logo_count : info_count;

    for (int i = 0; i < total_lines; i++) {
        const char *logo_part = (i < logo_count) ? logo[i] : "";
        const char *info_part = (i < info_count) ? info[i] : "";
        
        char line[256];
        if (info_part[0] != '\0')
            snprintf(line, sizeof(line), "%s\033[40G%s", logo_part, info_part);
        else
            snprintf(line, sizeof(line), "%s", logo_part);
        putline(ctx, line);
    }
}

// Wrapper for shell output.
static void uart_putline(void *ctx, const char *line) {
    (void)ctx;
    uart_puts(line);
    uart_puts("\r\n");
}

void sysinfo_print_uart(void) {
    sysinfo_print(uart_putline, NULL);
} //sysinfo_freertos.c