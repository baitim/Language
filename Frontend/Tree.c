#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"

FrontendErrorCode tocken_init(Tocken** tocken)
{
    *tocken = calloc(1, sizeof(Tocken));
    if (!(*tocken)) return FRONTEND_ERROR_ALLOC_FAIL;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tocken_delete(Tocken* tocken)
{
    free(tocken->argument);
    free(tocken);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tocken_dump(Tocken* tocken)
{
    fprintf(stderr, "%s\n", tocken->argument);

    return FRONTEND_ERROR_NO;
}

int data_destroy(void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    Tocken* tocken = (Tocken*)data;
    free(tocken->argument);

    return FRONTEND_ERROR_NO;
}

int data_verify(const void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Tocken* tocken = (const Tocken*)data;
    if (!tocken->argument) return FRONTEND_ERROR_NODE_DATA;

    return FRONTEND_ERROR_NO;
}

int data_dump_cmd(const void* data)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Tocken* tocken = (const Tocken*)data;
    fprintf(stderr, "%s", tocken->argument);

    return FRONTEND_ERROR_NO;
}

int data_dump_svg(const void* data, char** output_buffer)
{
    if (!data) return FRONTEND_ERROR_NODE_DATA;

    const Tocken* tocken = (const Tocken*)data;
    *output_buffer += sprintf(*output_buffer, "%s", tocken->argument);
    *output_buffer += sprintf(*output_buffer, "}\", fillcolor = \"#ab5b0f\"];\n");

    return FRONTEND_ERROR_NO;
}