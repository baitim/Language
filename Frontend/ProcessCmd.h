#ifndef FRONTEND_PROCESS_CMD_H
#define FRONTEND_PROCESS_CMD_H

#include "../ProcessCmd/ProcessCmd.h"

typedef struct _FrontendCmdInputData {
    int is_data_file;
    int is_help;
    char* name_data_file;
} FrontendCmdInputData;

CmdErrorCode data_file_callback(const char* argv[], void* data);
CmdErrorCode help_callback     (const char* /*argv*/[], void* data);

CmdErrorCode cmd_data_verify   (FrontendCmdInputData* cmd_data);
CmdErrorCode cmd_data_delete   (FrontendCmdInputData* cmd_data);

static const CmdLineOption FRONTEND_CMD_OPTIONS[] = {
    {"--name_data_file", "include file with program",   1, data_file_callback},
    {"--help",           "help",                        0, help_callback}
};
static const int COUNT_FRONTEND_CMD_OPTIONS = sizeof(FRONTEND_CMD_OPTIONS) / sizeof(CmdLineOption);

#endif // FRONTEND_PROCESS_CMD_H