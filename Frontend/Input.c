#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Input.h"

static FrontendErrorCode split_to_tockens   (FrontendInputData* input_data);
static FrontendErrorCode read_word          (char* buf, char* str);
static char* skip_spaces                    (char* str);
static char* skip_word                      (char* str);
static FrontendErrorCode file_to_buf        (const char* name_file, char** buf);
static FrontendErrorCode fsize              (const char* name_file, size_t* size_file);

FrontendErrorCode tree_input(Tree* tree)
{
    if (!tree) return FRONTEND_ERROR_TREE_NULL;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_init(FrontendInputData** input_data, char* name_file)
{
    FrontendErrorCode frontend_error = FRONTEND_ERROR_NO;

    *input_data = calloc(1, sizeof(FrontendInputData));
    if (!input_data) return FRONTEND_ERROR_ALLOC_FAIL;

    (*input_data)->count_tockens = 0;

    (*input_data)->name_file = strdup(name_file);
    if (!(*input_data)->name_file) return FRONTEND_ERROR_ALLOC_FAIL;

    frontend_error = file_to_buf((*input_data)->name_file, &(*input_data)->buf);
    if (frontend_error) return frontend_error;

    frontend_error = split_to_tockens(*input_data);
    if (frontend_error) return frontend_error;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_delete(FrontendInputData* input_data)
{
    if (!input_data) return FRONTEND_ERROR_NO;

    free(input_data->name_file);
    free(input_data->buf);

    for (int i = 0; i < input_data->count_tockens; i++)
        tocken_delete(input_data->tockens[i]);
    input_data->count_tockens = 0;

    free(input_data);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_dump(FrontendInputData* input_data)
{
    fprintf(stderr, "Input Data:\n");
    fprintf(stderr, "file: %s\n", input_data->name_file);
    fprintf(stderr, "buf:\n%s\n", input_data->buf);

    fprintf(stderr, "count tockens = %d\n", input_data->count_tockens);
    for (int i = 0; i < input_data->count_tockens; i++) {
        fprintf(stderr, "tocken: %d\t", i + 1);
        tocken_dump(input_data->tockens[i]);
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode split_to_tockens(FrontendInputData* input_data)
{
    if (!input_data) return FRONTEND_ERROR_NO;
    
    FrontendErrorCode frontend_error = FRONTEND_ERROR_NO;

    char* buf = input_data->buf;

    input_data->count_tockens = 0;
    while (*buf != '\0') {
        if (input_data->count_tockens == MAX_COUNT_TOCKENS) return FRONTEND_ERROR_MAX_COUNT_TOCKENS;
        
        buf = skip_spaces(buf);

        frontend_error = tocken_init(&input_data->tockens[input_data->count_tockens]);
        if (frontend_error) return frontend_error;

        input_data->tockens[input_data->count_tockens]->argument = calloc(1, MAX_SIZE_WORD);
        if (!input_data->tockens[input_data->count_tockens]->argument) return FRONTEND_ERROR_ALLOC_FAIL;

        frontend_error = read_word(buf, input_data->tockens[input_data->count_tockens]->argument);
        if (frontend_error) return frontend_error;


        buf = skip_word(buf);

        input_data->count_tockens++;

        buf = skip_spaces(buf);
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode read_word(char* buf, char* str)
{
    int i = 0;
    while (buf[i] != '\0' && !isspace(buf[i])) {
        if (i == MAX_SIZE_WORD - 1) return FRONTEND_ERROR_MAX_SIZE_WORD;
        str[i] = buf[i];
        i++;
    }
    str[i + 1] = '\0';

    return FRONTEND_ERROR_NO;
}

static char* skip_spaces(char* str)
{
    while (isspace(*str))
        str++;
    return str;
}

static char* skip_word(char* str)
{
    while (*str != '\0' && !isspace(*str))
        str++;
    return str;
}

static FrontendErrorCode file_to_buf(const char* name_file, char** buf)
{
    if (!name_file) return FRONTEND_ERROR_OPEN_FILE;

    FrontendErrorCode error = FRONTEND_ERROR_NO;

    FILE* data_file = fopen(name_file, "r");
    if (!data_file) return FRONTEND_ERROR_OPEN_FILE;

    size_t size_file = (size_t)-1;
    error = fsize(name_file, &size_file);
    if (error) return error;

    *buf = (char *)calloc(size_file, sizeof(char));
    if (!(*buf)) return FRONTEND_ERROR_ALLOC_FAIL;

    size_t count_read = fread(*buf, sizeof(char), size_file, data_file);
    if (count_read != size_file - 1) return FRONTEND_ERROR_READ_FILE;
    (*buf)[size_file - 1] = '\0';

    fclose(data_file);
    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode fsize(const char* name_file, size_t* size_file)
{
    struct stat st = {};

    if (stat(name_file, &st) == 0) {
        (*size_file) = (size_t)st.st_size + 1;
        return FRONTEND_ERROR_NO;
    }

    return FRONTEND_ERROR_READ_FILE;
}