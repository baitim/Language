#ifndef FRONTEND_TREE_H
#define FRONTEND_TREE_H

#include "Errors.h"

typedef struct _Tocken {
    char* argument;
} Tocken;

FrontendErrorCode tocken_init   (Tocken** tocken);
FrontendErrorCode tocken_delete (Tocken* tocken);
FrontendErrorCode tocken_dump   (Tocken* tocken);

int data_destroy  (void* data);
int data_verify   (const void* data);
int data_dump_cmd (const void* data);
int data_dump_svg (const void* data, char** output_buffer);

#endif // FRONTEND_TREE_H