#ifndef LOGO_H
#define LOGO_H

#define ANSI_CYAN   "\033[36m"
#define ANSI_BRIGHT_CYAN "\033[96m"
#define ANSI_RESET   "\033[0m"

static const char *logo[] = {
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    ANSI_BRIGHT_CYAN "     =+++++++++++++++++++=                   " ANSI_RESET,
    ANSI_CYAN "  *" ANSI_BRIGHT_CYAN "+++++++++++++++++++++++++++++             " ANSI_RESET,
    ANSI_CYAN " #**" ANSI_BRIGHT_CYAN "+++++++++++++++++++++++++++++++          " ANSI_RESET,
    ANSI_CYAN "###" ANSI_BRIGHT_CYAN "++++=++++                 ++++++++++      " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                    =++++    " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                       ++=+  " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                           ++" ANSI_RESET
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    ANSI_BRIGHT_CYAN "                                             " ANSI_RESET,
    NULL
};

#endif