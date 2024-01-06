#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Operators.h"
#include "Separators.h"
#include "Tree.h"

FrontendErrorCode token_init(Token** token)
{
    *token = calloc(1, sizeof(Token));
    if (!(*token)) return FRONTEND_ERROR_ALLOC_FAIL;

    (*token)->value = -1;
    (*token)->type = TYPE_ERR;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode token_delete(Token* token)
{
    free(token);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode token_dump_cmd(const Token* token)
{
    fprintf(stderr, "(%d %d)\n", token->value, token->type);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode token_dump_svg(const Token* token, char** output_buffer)
{
    *output_buffer += sprintf(*output_buffer, "%d | %d", token->value, token->type);
    *output_buffer += sprintf(*output_buffer, "}\", fillcolor = \"#ab5b0f\"];\n");

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode token_verify (const Token* token)
{
    if (token->type == TYPE_ERR) return FRONTEND_ERROR_NODE_DATA;
    if (token->type == TYPE_SEP && token->value == SEP_ERR) return FRONTEND_ERROR_NODE_DATA;
    if (token->type == TYPE_OP  && token->value == OP_ERR)  return FRONTEND_ERROR_NODE_DATA;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode token_copy(Token* token_dest, Token* token_src)
{
    token_dest->value = token_src->value;
    token_dest->type =  token_src->type;

    return FRONTEND_ERROR_NO;
}

int data_init(void** data_dest, void* data_src)
{
    if (!data_src) return FRONTEND_ERROR_NODE_DATA;

    *data_dest = calloc(1, sizeof(Token));
    if (!(*data_dest)) return FRONTEND_ERROR_ALLOC_FAIL;

    int error = token_copy((Token*)data_dest, (Token*)data_src);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}

int data_destroy(void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    Token* token = (Token*)data;
    int error = token_delete(token);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}

int data_verify(const void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Token* token = (const Token*)data;
    int error = token_verify(token);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}

int data_dump_cmd(const void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Token* token = (const Token*)data;
    int error = token_dump_cmd(token);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}

int data_dump_svg(const void* data, char** output_buffer)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Token* token = (const Token*)data;
    int error = token_dump_svg(token, output_buffer);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}