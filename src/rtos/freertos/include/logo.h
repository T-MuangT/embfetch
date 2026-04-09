#ifndef LOGO_H
#define LOGO_H

#define ANSI_BLUE  "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_RESET "\033[0m"

static const char *logo[] = {
    ANSI_BLUE "                                             " ANSI_RESET,
    ANSI_BLUE "                            =:::::::------=  " ANSI_RESET,
    ANSI_BLUE "                           ++++-:-------=++  " ANSI_RESET,
    ANSI_BLUE "                           ++++++=--==+++++  " ANSI_RESET,
    ANSI_BLUE "                          +++++++++++++++++  " ANSI_RESET,
    ANSI_BLUE "                         =++++++++****+++++  " ANSI_RESET,
    ANSI_BLUE "                         +++++++**##***++++  " ANSI_RESET,
    ANSI_BLUE "                        ++++++*#####*****++  " ANSI_RESET,
    ANSI_BLUE "                        ++++*#######******+  " ANSI_RESET,
    ANSI_MAGENTA " ======                " ANSI_BLUE "+++*########*****     " ANSI_RESET,
    ANSI_MAGENTA "  ====                 " ANSI_BLUE "+*##****+             " ANSI_RESET,
    ANSI_MAGENTA "    ==      ====    **+                      " ANSI_RESET,
    ANSI_MAGENTA "     -+++++======+###*                       " ANSI_RESET,
    ANSI_MAGENTA "      ++++          **                       " ANSI_RESET,
    ANSI_MAGENTA "                                             " ANSI_RESET
};

#endif