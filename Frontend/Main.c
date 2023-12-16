#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../ANSI_colors.h"
#include "../Errors/Errors.h"
#include "../Tree/Tree.h"

void int_destroy    (void* data);
int int_verify      (const void* data);
void int_dump_cmd   (const void* data);
void int_dump_svg   (const void* data, char** output_buffer);

void int_destroy(void* data)
{
    free(data);
}

int int_verify(const void* data)
{
    if (!data) return 1;
    return 0;
}

void int_dump_cmd(const void* data)
{
    fprintf(stderr, "%d", *(const int*)data);
}

void int_dump_svg(const void* data, char** output_buffer)
{
    *output_buffer += sprintf(*output_buffer, "%d", *(const int*)data);
    *output_buffer += sprintf(*output_buffer, "}\", fillcolor = \"#ab5b0f\"];\n");
}

int main()
{
    printf(print_lblue("# Implementation of language front-end.\n"
                       "# (c) BAIDUSENOV TIMUR, 2023\n\n"));

    int error = 0;
    Tree* tree = NULL;

    tree_init(&tree, "frontend_tree", int_destroy, int_verify, int_dump_cmd, int_dump_svg);
    error = tree_verify(tree);
    if (error) goto error;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 20; i++) {
        error = tree_insert(tree, (int)rand() % 50);
        if (error) goto error;

        error = tree_verify(tree);
        if (error) goto error;

        error = tree_dump(tree);
        if (error) goto error;
    }

    goto finally;
error:
    err_dump(error, ERROR_CODE_TREE);

finally:
    tree_destroy(tree);

    return error;
}