#include <nuttx/config.h>
#include "embed_sysinfo.h"

int main(int argc, FAR char *argv[])
{
    sysinfo_print_nuttx();
    return 0;
}