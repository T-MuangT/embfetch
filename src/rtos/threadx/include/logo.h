#ifndef LOGO_H
#define LOGO_H

#define ANSI_YELLOW "\033[33m"
#define ANSI_RESET  "\033[0m"

static const char *logo[] = {
    ANSI_YELLOW "                              " ANSI_RESET,
    ANSI_YELLOW "         ************         " ANSI_RESET,
    ANSI_YELLOW "     *******************      " ANSI_RESET,
    ANSI_YELLOW "    **********************    " ANSI_RESET,
    ANSI_YELLOW "  *****    *******     *****  " ANSI_RESET,
    ANSI_YELLOW " ***          **     ******** " ANSI_RESET,
    ANSI_YELLOW "**    " ANSI_RESET "-------" ANSI_YELLOW "      ***********" ANSI_RESET,
    ANSI_YELLOW "*  " ANSI_RESET "---------------" ANSI_YELLOW "   *********" ANSI_RESET,
    ANSI_RESET " ----------      ----  " ANSI_YELLOW "*******" ANSI_RESET,
    ANSI_RESET " --------    ---     --- " ANSI_YELLOW "*****" ANSI_RESET,
    ANSI_RESET "-------     ------     --- " ANSI_YELLOW "***" ANSI_RESET,
    ANSI_RESET "--------------------------- " ANSI_YELLOW "**" ANSI_RESET,
    ANSI_RESET " ----------------------------" ANSI_RESET,
    ANSI_RESET "  --------------------------  " ANSI_RESET,
    ANSI_RESET "    ----------------------    " ANSI_RESET,
    ANSI_RESET "      ------------------      " ANSI_RESET,
    ANSI_RESET "         ------------         " ANSI_RESET,
    ANSI_RESET "                              " ANSI_RESET,
    NULL
};

#endif