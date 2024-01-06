#ifndef NAME_TABLE_ERRORS_H
#define NAME_TABLE_ERRORS_H

#include "../Errors/Errors.h"

typedef enum _NameTableErrorCode {
    NAME_TABLE_ERROR_NO =          0,
    NAME_TABLE_ERROR_ALLOC_FAIL =  1 << 0
} NameTableErrorCode;

static const Error NAME_TABLE_ERRORS[] = {
    {NAME_TABLE_ERROR_NO,           "no errors"},
    {NAME_TABLE_ERROR_ALLOC_FAIL,   "alloc fail"}
};
static const int COUNT_NAME_TABLE_ERRORS = sizeof(NAME_TABLE_ERRORS) / sizeof(Error);

#endif // NAME_TABLE_ERRORS_H