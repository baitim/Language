#ifndef FRONTEND_TREE_H
#define FRONTEND_TREE_H

#include "Errors.h"

typedef enum _TokenDataType {
    TYPE_ERR = -1,
    TYPE_KW =   1,
    TYPE_NUM =  2,
    TYPE_OP  =  3,
    TYPE_SEP =  4,
    TYPE_DEC =  5,
    TYPE_ID =   6
} TokenDataType;

typedef struct _Token {
    int value;
    TokenDataType type;
} Token;

typedef struct _Tokens {
    Token** token;
    size_t count_tokens;
} Tokens;

FrontendErrorCode token_init       (Token** token);
FrontendErrorCode token_delete     (Token* token);
FrontendErrorCode token_dump_cmd   (const Token* token);
FrontendErrorCode token_dump_svg   (const Token* token, char** output_buffer);
FrontendErrorCode token_verify     (const Token* token);
FrontendErrorCode token_copy       (Token* dest_token, Token* src_token);

int data_init     (void** data_dest, void* data_src);
int data_destroy  (void* data);
int data_verify   (const void* data);
int data_dump_cmd (const void* data);
int data_dump_svg (const void* data, char** output_buffer);

#endif // FRONTEND_TREE_H