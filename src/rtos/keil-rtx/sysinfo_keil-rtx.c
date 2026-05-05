#include "cmsis_os2.h"
#include "rtx_os.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#if !defined(SYSINFO_NO_MALLINFO)
#  include <stdlib.h>
#endif
#include "rtos_sysinfo.h"
#include "logo.h"

//#include "board.h"        // define these data in your board header then uncomment
#ifndef SYSINFO_OS_NAME
#  define SYSINFO_OS_NAME    "Keil RTX5"
#endif
 
#ifndef SYSINFO_HOSTNAME     
#  define SYSINFO_HOSTNAME   "rtx5"
#endif
 
#ifndef SYSINFO_MCU          
#  define SYSINFO_MCU        "Unknown"
#endif

#ifndef SYSINFO_KERNEL_VER
#  define SYSINFO_KERNEL_VER "5.9.1"
#endif

#if !defined(SYSINFO_NO_MALLINFO) && !defined(__MICROLIB)
#  include <stdlib.h>
#endif

#ifndef SYSINFO_SRAM_KB      
#  define SYSINFO_SRAM_KB    0u
#endif
 
#ifndef SYSINFO_FLASH_KB     
#  define SYSINFO_FLASH_KB   0u
#endif

// Static board info fetching
static const sysinfo_static_t board_info = {
    .username   = "root",
    .hostname   = SYSINFO_HOSTNAME,
    .os_name    = SYSINFO_OS_NAME,
    .mcu        = SYSINFO_MCU,
    .build_date = __DATE__ " " __TIME__,
};

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes)
{
    if (bytes >= 1024u * 1024u) {
        snprintf(dst, len, "%uMB", (unsigned)(bytes >> 20));
    } else if (bytes >= 1024u) {
        snprintf(dst, len, "%uKB", (unsigned)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB",  (unsigned)bytes);
    }
}

// Hardware info fetching
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    // RAM
    if (SYSINFO_SRAM_KB > 0u) {
        format_size(dst->ram, sizeof(dst->ram), (size_t)SYSINFO_SRAM_KB * 1024u);
    } else {
        snprintf(dst->ram, sizeof(dst->ram), "Unknown");
    }
 
    // Flash
    if (SYSINFO_FLASH_KB > 0u) {
        format_size(dst->flash, sizeof(dst->flash), (size_t)SYSINFO_FLASH_KB * 1024u);
    } else {
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
    }
}

// Dynamic info fetching
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
	  uint32_t ticks, tick_hz, uptime_s;
    // Kernel version
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "RTX5 %s", SYSINFO_KERNEL_VER);
 
    // Uptime
    ticks    = osKernelGetTickCount();
    tick_hz  = osKernelGetTickFreq();
    if (tick_hz == 0u) tick_hz = 1u;	// Guard divide-by-zero error
    uptime_s  = ticks / tick_hz;
    dst->uptime_h = uptime_s / 3600u;
    dst->uptime_m = (uptime_s % 3600u) / 60u;
    dst->uptime_s =  uptime_s % 60u;
 
    // Memory heap
#if defined(SYSINFO_NO_MALLINFO) || defined(__MICROLIB)
    snprintf(dst->heap_used, sizeof(dst->heap_used), "Unknown");
    snprintf(dst->heap_free, sizeof(dst->heap_free), "Unknown");
#elif defined(__STDC_HOSTED__) && __STDC_HOSTED__ == 1
    {
        struct mallinfo mi;
        size_t heap_used;
        size_t heap_free;
        mi        = mallinfo();
        heap_used = (size_t)mi.uordblks;
        heap_free = (size_t)mi.fordblks;
        format_size(dst->heap_used, sizeof(dst->heap_used), heap_used);
        format_size(dst->heap_free, sizeof(dst->heap_free), heap_free);
    }
#else
    snprintf(dst->heap_used, sizeof(dst->heap_used), "Unknown");
    snprintf(dst->heap_free, sizeof(dst->heap_free), "Unknown");
#endif
}

// Print logo and info, rearranged from normal due to C99 compliance requirements
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    static sysinfo_dynamic_t dyn;
    static sysinfo_hwinfo_t  hw;
    static char header[64],     separator[32];
    static char os_line[64],    kernel_line[64], mcu_line[64],    build_line[64];
    static char flash_line[64], ram_line[64],    uptime_line[64], heap_line[64];
    static char line[256];
    int logo_count, info_count, total, i;
 
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_fetch(&hw);
 
    snprintf(header,      sizeof(header),      "%s@%s", board_info.username, board_info.hostname);
    snprintf(separator,   sizeof(separator),   "----------------");
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", board_info.mcu);
    snprintf(ram_line,    sizeof(ram_line),    "Memory:  %s", hw.ram);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free", dyn.heap_used, dyn.heap_free);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", hw.flash);
 
    // All data lines
    {
        const char *info[] = {
            header,      separator,  os_line,    kernel_line,
            uptime_line, build_line, mcu_line,   ram_line,
            heap_line,   flash_line
        };
 
        logo_count = 0;
        while (logo[logo_count] != NULL) logo_count++;
        info_count = (int)(sizeof(info) / sizeof(info[0]));
        total      = (logo_count > info_count) ? logo_count : info_count;
 
        for (i = 0; i < total; i++) {
            const char *logo_part = (i < logo_count) ? logo[i] : "";
            const char *info_part = (i < info_count) ? info[i] : "";
 
            if (info_part[0] != '\0')
                snprintf(line, sizeof(line), "%s\033[40G%s", logo_part, info_part);
            else
                snprintf(line, sizeof(line), "%s", logo_part);
            putline(ctx, line);
        }
    }
}

// Wrapper for shell output
static void (*saved_uart_puts)(const char *) = 0;

static void rtx_puts_putline(void *ctx, const char *line) {
    (void)ctx;
    if (saved_uart_puts) saved_uart_puts(line);
}

void sysinfo_print_rtx(void (*uart_puts)(const char *)) {
    saved_uart_puts = uart_puts;
    sysinfo_print(rtx_puts_putline, 0);
} //sysinfo_keil-rtx.c