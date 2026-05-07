if(CONFIG_EMBFETCH)
    zephyr_library()
    zephyr_library_sources(
        ../src/sysinfo_rtos_common.c
        ../src/rtos/zephyr/sysinfo_zephyr.c
    )
    zephyr_include_directories(../include)
endif()