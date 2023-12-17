#ifndef TREE_ERRORS_H
#define TREE_ERRORS_H

#include "../Errors/Errors.h"

typedef enum _TreeErrorCode {
    TREE_ERROR_NO =             0,
    TREE_ERROR_ALLOC_FAIL =     1 << 0,
    TREE_ERROR_SYSTEM_COMMAND = 1 << 1,
    TREE_ERROR_OPEN_FILE =      1 << 2,
    TREE_ERROR_LOOP =           1 << 3,
    TREE_ERROR_DEPTH =          1 << 4,
    TREE_ERROR_NODE_DATA =      1 << 5
} TreeErrorCode;

static const Error TREE_ERRORS[] = {
    {TREE_ERROR_NO,             "no errors"},
    {TREE_ERROR_ALLOC_FAIL,     "alloc fail"},
    {TREE_ERROR_SYSTEM_COMMAND, "process system command failed"},
    {TREE_ERROR_OPEN_FILE,      "opening file failed"},
    {TREE_ERROR_LOOP,           "tree has loop"},
    {TREE_ERROR_DEPTH,          "wrong depth"},
    {TREE_ERROR_NODE_DATA,      "wrong data in node"}
};
static const int COUNT_TREE_ERRORS = sizeof(TREE_ERRORS) / sizeof(Error);

#endif // TREE_ERRORS_H