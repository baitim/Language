#ifndef FRONTEND_INPUT_H
#define FRONTEND_INPUT_H

#include "Errors.h"
#include "Tree.h"
#include "../Tree/Tree.h"

#define MAX_COUNT_TOCKENS 10000
#define MAX_SIZE_WORD 100

typedef struct _FrontendInputData {
    char* name_file;
    char* buf;
    Tocken* tockens[MAX_COUNT_TOCKENS];
    int count_tockens;
} FrontendInputData;

FrontendErrorCode input_data_init   (FrontendInputData** input_data, char* name_file);
FrontendErrorCode input_data_delete (FrontendInputData* input_data);
FrontendErrorCode input_data_dump   (FrontendInputData* input_data);
FrontendErrorCode tree_input        (Tree* tree);

#endif // FRONTEND_INPUT_H