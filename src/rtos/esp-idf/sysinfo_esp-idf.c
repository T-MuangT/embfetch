#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "embed_sysinfo.h"
#include "logo.h"
#include "esp_idf_version.h"
#include <inttypes.h>

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
#include "esp_flash.h"
#else
#include "esp_spi_flash.h"  // legacy header, provides spi_flash_get_chip_size()
#endif

const char *mcu_name;
const char *hostname;
static void chip_info_init(const char **mcu_name, const char **hostname) {
	esp_chip_info_t chip;
	esp_chip_info(&chip);

	switch (chip.model) {
	    case CHIP_ESP32:   *mcu_name = "ESP32";   *hostname = "esp32";    break;
	    case CHIP_ESP32S2: *mcu_name = "ESP32-S2"; *hostname = "esp32s2";  break;
	    case CHIP_ESP32S3: *mcu_name = "ESP32-S3"; *hostname = "esp32s3";  break;
	    case CHIP_ESP32C2: *mcu_name = "ESP32-C2"; *hostname = "esp32c2";  break;
	    case CHIP_ESP32C3: *mcu_name = "ESP32-C3"; *hostname = "esp32c3";  break;
	    case CHIP_ESP32C5: *mcu_name = "ESP32-C5"; *hostname = "esp32c5";  break;
	    case CHIP_ESP32C6: *mcu_name = "ESP32-C6"; *hostname = "esp32c6";  break;
	    case CHIP_ESP32C61: *mcu_name = "ESP32-C61"; *hostname = "esp32c61";  break;
	    case CHIP_ESP32H2: *mcu_name = "ESP32-H2"; *hostname = "esp32h2";  break;
	    case CHIP_ESP32P4: *mcu_name = "ESP32-P4"; *hostname = "esp32p4";  break;
    	default:           *mcu_name = "Unknown";  *hostname = "unknown";  break;
	}
}

// Static board info fetching.
static const sysinfo_static_t board_info = {
    .username       = "root",
    .os_name        = "ESP-IDF",
	.build_date		= __DATE__" "__TIME__,
};

// Byte formatting.
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%uMB", (unsigned int)(bytes >> 20));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%uKB", (unsigned int)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB", (unsigned int)bytes);
    }
}

// Hardware info fetching. 
// No devicetree, but the ESP-IDF API can provide flash and memory sizes.
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    // RAM
    size_t ram = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    format_size(dst->ram, sizeof(dst->ram), ram);

    // Flash
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    uint32_t flash_size;
    esp_flash_get_size(NULL, &flash_size);
#else
    uint32_t flash_size = spi_flash_get_chip_size();
#endif
    format_size(dst->flash, sizeof(dst->flash), flash_size);
}

// Dynamic info fetching.
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Kernel version
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "FreeRTOS %d.%d.%d", tskKERNEL_VERSION_MAJOR, tskKERNEL_VERSION_MINOR, tskKERNEL_VERSION_BUILD);

    // Uptime from esp_timer
    // NOTE: esp_timer does not loop back.
    // For long-running uptime, maintain a separate overflow counter in a timer callback.
    int64_t uptime_us = esp_timer_get_time();
    uint64_t uptime_s = (uint64_t)(uptime_us / 1000000);
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // Heap stats
    // ESP-IDF, based on FreeRTOS, thus only exposes free heap natively; used = total - free.
    // heap_4/heap_5 also provide heap_caps_get_minimum_free_size() for low-watermark.
    size_t heap_free  = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t heap_total = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    size_t heap_used  = heap_total - heap_free;

    format_size(dst->heap_used, sizeof(dst->heap_used), heap_used);
    format_size(dst->heap_free, sizeof(dst->heap_free), heap_free);
}

// Print logo and info.
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
	chip_info_init(&mcu_name, &hostname);
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_t hw;
    sysinfo_hwinfo_fetch(&hw);

    char header[64], separator[32];
    snprintf(header, sizeof(header), "%s@%s", board_info.username, hostname);
    snprintf(separator, sizeof(separator), "----------------");

    // All your data lines
    char os_line[64], kernel_line[64], mcu_line[64], build_line[64], 
         flash_line[64], ram_line[64], uptime_line[64], heap_line[64];

    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
	snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %" PRIu32 "h %" PRIu32 "m %" PRIu32 "s", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", mcu_name);
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
            snprintf(line, sizeof(line), "%s\033[60G%s", logo_part, info_part);
        else
            snprintf(line, sizeof(line), "%s", logo_part);
        putline(ctx, line);
    }
}

// Wrapper for shell output.
static void uart_putline(void *ctx, const char *line) {
    (void)ctx;
    printf("%s\r\n", line);
}

void sysinfo_print_espidf(void) {
    sysinfo_print(uart_putline, NULL);
} //sysinfo_esp-idf.c