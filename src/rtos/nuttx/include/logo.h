#ifndef LOGO_H
#define LOGO_H

#define ANSI_BLUE   "\033[34m"
#define ANSI_RESET   "\033[0m"

#define LOGO_INFO_COLUMN "\033[50G"

static const char *logo[] = {
    ANSI_BLUE "                  #***                  " ANSI_RESET,
    ANSI_BLUE "                #*******                " ANSI_RESET,
    ANSI_BLUE "              #***********              " ANSI_RESET,
    ANSI_BLUE "       " ANSI_RESET "---- " ANSI_BLUE "#*****" ANSI_RESET "+==+" ANSI_BLUE "*****+" ANSI_RESET "  ----=" ANSI_BLUE "     " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::=" ANSI_BLUE "*******" ANSI_RESET "=:::+" ANSI_BLUE "******" ANSI_RESET "=-::=*#%" ANSI_BLUE "   " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-::::=" ANSI_BLUE "#*****" ANSI_RESET "=::::+#" ANSI_BLUE "****" ANSI_RESET "-::-*##    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::::=" ANSI_BLUE "#****" ANSI_RESET "=:::::+#" ANSI_BLUE "**" ANSI_RESET "-::-*##     " ANSI_RESET,
    ANSI_BLUE "    #*" ANSI_RESET "+::::::=" ANSI_BLUE "#***" ANSI_RESET "=::::::+*-::-*##" ANSI_BLUE "**    " ANSI_RESET,
    ANSI_BLUE "  #***" ANSI_RESET "+:::=-::=" ANSI_BLUE "#**" ANSI_RESET "=::-=:::-::-*##" ANSI_BLUE "*****  " ANSI_RESET,
    ANSI_BLUE "#*****" ANSI_RESET "+:::*+-::=" ANSI_BLUE "#*" ANSI_RESET "=::-" ANSI_BLUE "*" ANSI_RESET "+::::-##" ANSI_BLUE "*********" ANSI_RESET,
    ANSI_BLUE "+*****" ANSI_RESET "+:::*" ANSI_BLUE "#" ANSI_RESET "+:::=" ANSI_BLUE "*" ANSI_RESET "=::-" ANSI_BLUE "**" ANSI_RESET "-:::=##" ANSI_BLUE "********#" ANSI_RESET,
    ANSI_BLUE "  ****" ANSI_RESET "+:::*" ANSI_BLUE "#*" ANSI_RESET "+:::==::-" ANSI_BLUE "+" ANSI_RESET "-:::::=#" ANSI_BLUE "*******  " ANSI_RESET,
    ANSI_BLUE "    **" ANSI_RESET "+:::*" ANSI_BLUE "#**" ANSI_RESET "+-::-::-:::-+:::=#" ANSI_BLUE "***#    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#***" ANSI_RESET "+-:::::::-##+:::=##      " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#****" ANSI_RESET "+-:::::-*#" ANSI_BLUE "**" ANSI_RESET "+:::=#      " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-:::*" ANSI_BLUE "#*****" ANSI_RESET "+-:::=*#" ANSI_BLUE "****" ANSI_RESET  "=:::+#%    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "-::-*" ANSI_BLUE "#******" ANSI_RESET "+--=*#" ANSI_BLUE "***#  " ANSI_RESET "---+##" ANSI_BLUE "    " ANSI_RESET,
    ANSI_BLUE "      " ANSI_RESET "%\\###" ANSI_BLUE "  *******##**#        " ANSI_RESET "%\\#%" ANSI_BLUE "     " ANSI_RESET,
    ANSI_BLUE "                *******#                " ANSI_RESET,
    ANSI_BLUE "                  ***#                  " ANSI_RESET,
    NULL
};

#endif