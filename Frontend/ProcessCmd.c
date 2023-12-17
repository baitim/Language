#include <string.h>
#include <stdlib.h>

#include "ProcessCmd.h"

CmdErrorCode data_file_callback(const char* argv[], void* data)
{
    FrontendCmdInputData* cmd_data = data;
    cmd_data->is_data_file = 1;
    cmd_data->name_data_file = strdup(argv[1]);
    if (!cmd_data->name_data_file) return CMD_ERROR_ALLOC_FAIL;

    return CMD_ERROR_NO;
}

CmdErrorCode help_callback(const char* /*argv*/[], void* data)
{
    FrontendCmdInputData* cmd_data = data;
    cmd_data->is_help = 1;

    return CMD_ERROR_NO;
}

CmdErrorCode cmd_data_verify(FrontendCmdInputData* cmd_data)
{
    if (!cmd_data->is_data_file || !cmd_data->name_data_file) return CMD_ERROR_INVALID_FILE;
    
    return CMD_ERROR_NO;
}

CmdErrorCode cmd_data_delete(FrontendCmdInputData* cmd_data)
{
    free(cmd_data->name_data_file);

    return CMD_ERROR_NO;
}