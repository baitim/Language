#ifndef NAME_TABLE_NAME_TABLE_H
#define NAME_TABLE_NAME_TABLE_H

#include <stdio.h>

#include "Errors.h"

#define DEFAULT_NAME_TABLE_CAPACITY 1
#define MULTIPLIER_CAPACITY         2

typedef struct _NameTable {
    char** name;
    size_t count;
    size_t capacity;
} NameTable;

NameTableErrorCode name_table_init   (NameTable** name_table);
NameTableErrorCode name_table_delete (NameTable* name_table);
NameTableErrorCode name_table_dump   (const NameTable* name_table);

NameTableErrorCode name_table_increase       (NameTable* name_table);
NameTableErrorCode name_table_resize_capacity(NameTable* name_table, size_t new_capacity);

#endif // NAME_TABLE_NAME_TABLE_H