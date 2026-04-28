#ifndef LOGO_H
#define LOGO_H

#define ANSI_GREEN   "\033[32m"
#define ANSI_RESET   "\033[0m"

static const char *logo[] = {
    ANSI_GREEN "----------------------------------" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET "  _____ _______ ____   _____  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| f" ANSI_RESET " |  __ \\__   __/ __ \\ / ____| " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| r" ANSI_RESET " | |__> | | | | |  | | <___   " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| e" ANSI_RESET " |  _  /  | | | |  | |\\___ \\  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "| e" ANSI_RESET " | | \\ \\  | | | |__| |____> | " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET " |_|  \\_\\ |_|  \\____/|_____/  " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "|  " ANSI_RESET "                              " ANSI_GREEN "|" ANSI_RESET,
    ANSI_GREEN "----------------------------------" ANSI_RESET,
    NULL
};

#endif