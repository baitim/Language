#include <string.h>

#include "ProcessCmd.h"

typedef struct _OptionsSource {
    TypeCmdOptions type;
    const CmdLineOption* options;
    const int count;
} OptionsSource;

#include "../Frontend/ProcessCmd.h"

static const OptionsSource OPTIONS_SOURCES[] = {
    {TYPE_CMD_OPTIONS_NO,       NULL,                   0},
    {TYPE_CMD_OPTIONS_FRONTEND, FRONTEND_CMD_OPTIONS,   COUNT_FRONTEND_CMD_OPTIONS},
};

CmdErrorCode input_cmd(int argc, const char* argv[], void* cmd_data, TypeCmdOptions type_cmd_options)
{
    CmdErrorCode err = CMD_ERROR_NO;

    if (type_cmd_options == TYPE_CMD_OPTIONS_NO) return CMD_ERROR_NO;

    const CmdLineOption* options = OPTIONS_SOURCES[type_cmd_options].options;
    const int count_options = OPTIONS_SOURCES[type_cmd_options].count;

    for (int i = 0; i < argc; i++) {
        for (int j = 0; j < count_options; j++) {
        	if (strcmp(argv[i], options[j].name) == 0) {
                err = (*options[j].callback)(&argv[i], cmd_data);
                if (err) return err;
                i += options[j].n_args;
                break;
            }
        }
    }
    return err;
}