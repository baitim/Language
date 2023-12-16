#ifndef ERRORS_ERRORS_H
#define ERRORS_ERRORS_H

typedef struct _Error {
    int error;
    const char* description;
} Error;

typedef enum _ErrorType {
    ERROR_CODE_NO =     0,
    ERROR_CODE_TREE =   1 << 0         
} ErrorType;

#define err_dump(error, error_type) err_dump_(error, error_type, __FILE__, __PRETTY_FUNCTION__, __LINE__) 
void err_dump_(int error, ErrorType error_type, const char* file, const char* func, int line);

#endif // ERRORS_ERRORS_H