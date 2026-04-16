#ifndef EMBED_SYSINFO_H
#define EMBED_SYSINFO_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef void (*sysinfo_putline_fn)(void *ctx, const char *line);

#if defined(__ZEPHYR__)
#include <zephyr/shell/shell.h>
    void sysinfo_print_shell(const struct shell *sh);
#elif defined(RT_THREAD_ID) || defined(RT_USING_FINSH)
    void sysinfo_print_rt(void);
#elif defined(ESP_PLATFORM)
    void sysinfo_print_espidf(void);
#elif defined(FREERTOS_CONFIG_H) || defined(INC_FREERTOS_H)
    void sysinfo_print_uart(void);
#endif

// Static board info.
typedef struct {
    const char *username;       // "root", not actual username.
    const char *os_name;
	const char *build_date;
#if !defined(ESP_PLATFORM)
    const char *mcu;			// Runtime on ESP-IDF via esp_chip_info() will have both mcu and hostname info.
    const char *hostname;   	// Name of board, not actual hostname.
#endif
} sysinfo_static_t;

//Hardware info.
typedef struct {
    char flash[16];
    char ram[16];
} sysinfo_hwinfo_t;

// Dynamic runtime info.
typedef struct {
    char     kernel_version[32];
    uint32_t uptime_h;
    uint32_t uptime_m;
    uint32_t uptime_s;
    char     heap_used[16];
    char     heap_free[16];
} sysinfo_dynamic_t;

// Public API
// Fetch hardware info and dynamic info into dst, print logo and info using putline callback.
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst);
void sysinfo_fetch(sysinfo_dynamic_t *dst);
void sysinfo_print(sysinfo_putline_fn putline, void *ctx);

// Wrapper for output target.
void sysinfo_print_file(FILE *f);
void sysinfo_print_puts(void (*puts_fn)(const char *));

#endif // EMBED_SYSINFO_H