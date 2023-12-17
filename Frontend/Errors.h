#ifndef FRONTEND_ERRORS_H
#define FRONTEND_ERRORS_H

#include "../Errors/Errors.h"

typedef enum _FrontendErrorCode {
    FRONTEND_ERROR_NO =                 0,
    FRONTEND_ERROR_NODE_DATA =          1 << 0,
    FRONTEND_ERROR_TREE_NULL =          1 << 1,
    FRONTEND_ERROR_OPEN_FILE =          1 << 2,
    FRONTEND_ERROR_ALLOC_FAIL =         1 << 3,
    FRONTEND_ERROR_READ_FILE =          1 << 4,
    FRONTEND_ERROR_MAX_COUNT_TOCKENS =  1 << 5,
    FRONTEND_ERROR_MAX_SIZE_WORD =      1 << 6
} FrontendErrorCode;

static const Error FRONTEND_ERRORS[] = {
    {FRONTEND_ERROR_NO,                 "no errors"},
    {FRONTEND_ERROR_NODE_DATA,          "error in node data"},
    {FRONTEND_ERROR_TREE_NULL,          "tree is NULL"},
    {FRONTEND_ERROR_OPEN_FILE,          "open file"},
    {FRONTEND_ERROR_ALLOC_FAIL,         "alloc fail"},
    {FRONTEND_ERROR_READ_FILE,          "read file"},
    {FRONTEND_ERROR_MAX_COUNT_TOCKENS,  "too many words are used"},
    {FRONTEND_ERROR_MAX_SIZE_WORD,      "word longer than available"}
};
static const int COUNT_FRONTEND_ERRORS = sizeof(FRONTEND_ERRORS) / sizeof(Error);

#endif // FRONTEND_ERRORS_H