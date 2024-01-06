#ifndef FRONTEND_INPUT_H
#define FRONTEND_INPUT_H

#include "Errors.h"
#include "../NameTable/NameTable.h"
#include "Tree.h"
#include "../Tree/Tree.h"

#define MAX_COUNT_TOKENS 10000
#define MAX_SIZE_WORD    100

typedef struct _FrontendInputData {
    char* name_file;
    char* buf;
    char* words[MAX_COUNT_TOKENS];
    size_t count_words;
} FrontendInputData;

FrontendErrorCode input_data_init   (FrontendInputData** input_data, char* name_file);
FrontendErrorCode input_data_delete (FrontendInputData* input_data);
FrontendErrorCode input_data_dump   (FrontendInputData* input_data);
FrontendErrorCode tokenize          (FrontendInputData* input_data, Tokens** tokens, NameTable* name_table);
FrontendErrorCode tokens_init       (Tokens** tokens, size_t count_tokens);
FrontendErrorCode tokens_delete     (Tokens* tokens);
FrontendErrorCode tokens_dump       (Tokens* tokens);
FrontendErrorCode tree_input        (Tree* tree, Tokens* tokens, NameTable* name_table);

#endif // FRONTEND_INPUT_H