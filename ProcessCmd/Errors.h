#ifndef PROCESS_CMD_ERRORS_H
#define PROCESS_CMD_ERRORS_H

#include "../Errors/Errors.h"

typedef enum _CmdErrorCode {
    CMD_ERROR_NO =              0,
    CMD_ERROR_ALLOC_FAIL =      1 << 0,
    CMD_ERROR_INVALID_FILE =    1 << 1,
    CMD_ERROR_MAX_SIZE =        1 << 2
} CmdErrorCode;

static const Error CMD_ERRORS[] = {
    {CMD_ERROR_NO,              "no errors"},
    {CMD_ERROR_ALLOC_FAIL,      "alloc fail"},
    {CMD_ERROR_INVALID_FILE,    "invalid file"},
    {CMD_ERROR_MAX_SIZE,        "excess than max available size"}
};
static const int COUNT_CMD_ERRORS = sizeof(CMD_ERRORS) / sizeof(Error);

#endif // PROCESS_CMD_ERRORS_H