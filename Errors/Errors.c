#include <stdio.h>

#include "../ANSI_colors.h"
#include "Errors.h"

typedef struct _ErrorSource {
    ErrorType error_type;
    char* name_source;
    const Error* errors;
    int count;
} ErrorSource;

#include "../Tree/Errors.h"

const ErrorSource ERROR_SOURCES[] = {
    {ERROR_CODE_NO,     "null", NULL,           0},
    {ERROR_CODE_TREE,   "tree", TREE_ERRORS,    COUNT_TREE_ERRORS},
};
const int COUNT_ERROR_SOURCES = sizeof(ERROR_SOURCES) / sizeof(ErrorSource);

static void print_error(int error, const char* source, const char* description);

void err_dump_(int error, ErrorType error_type, const char* file,
               const char* func, int line)
{
    if (error_type == ERROR_CODE_NO) return;

    if (error == 0) {
        // fprintf(stderr, print_lgreen("dump: OK\n"));
        return;
    }

    fprintf(stderr, print_lred("ERROR: called from FILE = %s, FUNCTION = %s, LINE = %d\n"), 
                               file, func, line);

    const Error* errors = ERROR_SOURCES[error_type].errors;
    const int COUNT_ERRORS = ERROR_SOURCES[error_type].count;
    int pow = 0;
    for (int i = 0; i < COUNT_ERRORS; i++) {
        if (error & pow)
            print_error(error, ERROR_SOURCES[error_type].name_source, errors[i].description);
            
        if (!pow)   pow++;
        else        pow *= 2;
    }
}

static void print_error(int error, const char* source, const char* description)
{
    fprintf(stderr, print_lred("ERROR: %d %s %s\n"), error, source, description);
}