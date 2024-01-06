#include <stdlib.h>

#include "NameTable.h"

NameTableErrorCode name_table_init(NameTable** name_table)
{
    *name_table = calloc(1, sizeof(NameTable));
    if (!(*name_table)) return NAME_TABLE_ERROR_ALLOC_FAIL;

    (*name_table)->count =    0;
    (*name_table)->capacity = DEFAULT_NAME_TABLE_CAPACITY;

    (*name_table)->name = calloc((*name_table)->capacity, sizeof(char*));
    if (!(*name_table)->name) return NAME_TABLE_ERROR_ALLOC_FAIL;

    return NAME_TABLE_ERROR_NO;
}

NameTableErrorCode name_table_delete(NameTable* name_table)
{
    for (size_t i = 0; i < name_table->capacity; i++)
        free(name_table->name[i]);

    free(name_table->name);
    free(name_table);

    return NAME_TABLE_ERROR_NO;
}

NameTableErrorCode name_table_dump(const NameTable* name_table)
{
    fprintf(stderr, "Name table:\n");

    fprintf(stderr, "count names = %zu\n", name_table->capacity);
    for (size_t i = 0; i < name_table->capacity; i++)
        fprintf(stderr, "name: %zu\t%s\n", i + 1, name_table->name[i]);

    return NAME_TABLE_ERROR_NO;
}

NameTableErrorCode name_table_increase(NameTable* name_table)
{
    NameTableErrorCode error = name_table_resize_capacity(name_table, name_table->capacity * MULTIPLIER_CAPACITY);
    if (error) return error;

    return NAME_TABLE_ERROR_NO;
}

NameTableErrorCode name_table_resize_capacity(NameTable* name_table, size_t new_capacity)
{
    name_table->capacity = new_capacity;

    name_table->name = realloc(name_table->name, new_capacity * sizeof(char*));
    if (!name_table->name) return NAME_TABLE_ERROR_ALLOC_FAIL;

    return NAME_TABLE_ERROR_NO;
}