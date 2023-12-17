#ifndef PROCESS_CMD_PROCESS_CMD_H
#define PROCESS_CMD_PROCESS_CMD_H

#include "Errors.h"

#define MAX_CMD_COMMAND_SIZE 50
#define MAX_COUNT_OPTIONS 100

typedef enum _TypeCmdOptions {
    TYPE_CMD_OPTIONS_NO =       0,
    TYPE_CMD_OPTIONS_FRONTEND = 1 << 0
} TypeCmdOptions;

typedef struct _CmdLineOption {
    const char* name;
    const char* description;
    int n_args;
    CmdErrorCode (*callback)(const char* argv[], void* data);
} CmdLineOption;

CmdErrorCode input_cmd(int argc, const char* argv[], void* cmd_data, TypeCmdOptions type_cmd_options);

#endif // PROCESS_CMD_PROCESS_CMD_H