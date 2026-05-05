#include <nuttx/config.h>
#include "rtos_sysinfo.h"

int main(int argc, FAR char *argv[])
{
    sysinfo_print_nuttx();
    return 0;
}