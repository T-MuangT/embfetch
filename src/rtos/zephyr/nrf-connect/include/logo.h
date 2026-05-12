#ifndef LOGO_H
#define LOGO_H
 
#define ANSI_CYAN  "\033[36m"
#define ANSI_RESET "\033[0m"

#define LOGO_INFO_COLUMN "\033[50G"
 
static const char *logo[] = {
    ANSI_CYAN "       ++++++              :::+++       " ANSI_RESET,
    ANSI_CYAN "    ++++++++++++       :::::::++++++    " ANSI_RESET,
    ANSI_CYAN " +++++++++++++++++++::::::::::++++++++++" ANSI_RESET,
    ANSI_CYAN "-=+++++++++++++++++++=::::::::++++++++++" ANSI_RESET,
    ANSI_CYAN "::::=+++++++++++++++++++=:::::++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::=++++++++++++++++++++-:++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++++++++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::-++++++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::::::::-=++++++++++++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::-=+++++++++++++++++++ " ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::::::=++++++++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::::::::::::::::::=+++++++++++++" ANSI_RESET,
    ANSI_CYAN ":::::::::::::::::::::::::::::-++++++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::==::::::::::::::::::::-=++++++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++=-::::::::::::::::::::=+++" ANSI_RESET,
    ANSI_CYAN "::::::::::=+++++++-::::::::::::::::::::=" ANSI_RESET,
    ANSI_CYAN " :::::::::=+++++++++ :::::::::::::::::: " ANSI_RESET,
    ANSI_CYAN "    ::::::=+++++        ::::::::::::    " ANSI_RESET,
    ANSI_CYAN "       :::=++              ::::::       " ANSI_RESET,
    NULL
};
 
#endif /* LOGO_H */