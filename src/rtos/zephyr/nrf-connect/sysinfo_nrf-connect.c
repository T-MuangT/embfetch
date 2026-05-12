#include <zephyr/kernel.h>
#include "rtos_sysinfo.h"

// TODO: add nRF Connect SDK specific includes into sysinfo_nrf-connect-connect.c
// e.g. #include <modem/nrf_modem_lib.h>
//      #include <modem/lte_lc.h>

// Static board info fetching
const sysinfo_static_t board_info = {
    .username   = "root",
    .hostname   = CONFIG_BOARD,
    .os_name    = "nRF Connect",
    .mcu        = CONFIG_SOC,
    .build_date = __DATE__ " " __TIME__,
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

// Hardware info — nRF Connect SDK specific APIs
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    // RAM
#if defined(CONFIG_SRAM_SIZE) && CONFIG_SRAM_SIZE > 0
    format_size(dst->ram, sizeof(dst->ram), CONFIG_SRAM_SIZE * 1024);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

    // Flash
#if defined(CONFIG_FLASH_SIZE) && CONFIG_FLASH_SIZE > 0
    format_size(dst->flash, sizeof(dst->flash), CONFIG_FLASH_SIZE * 1024);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif

    // TODO: nRF Connect specific fields
    // Modem firmware version via nrf_modem_at_cmd()
    // LTE band and signal info via lte_lc_conn_eval_params_get()
    // PMIC/battery info on nRF9160 DK
    // Bootloader/FOTA slot info via dfu_target
} // sysinfo_nrf-connect.c