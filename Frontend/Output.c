#include <stdio.h>

#include "../ANSI_colors.h"
#include "Output.h"

CmdErrorCode print_help()
{
    for (int i = 0; i < COUNT_FRONTEND_CMD_OPTIONS; i++) {
        int count_print = 0;
        printf(print_lgreen("%s%n"), FRONTEND_CMD_OPTIONS[i].name, &count_print);

        if (count_print > MAX_CMD_COMMAND_SIZE) return CMD_ERROR_MAX_SIZE;

        for (int j = 0; j <= MAX_CMD_COMMAND_SIZE - count_print + 1; j++)
            printf(" ");
        printf(print_lgreen("%s\n"), FRONTEND_CMD_OPTIONS[i].description);
    }

    return CMD_ERROR_NO;
}