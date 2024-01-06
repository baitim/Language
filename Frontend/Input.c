#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../ANSI_colors.h"
#include "Declarators.h"
#include "Input.h"
#include "KeyWords.h"
#include "Operators.h"
#include "Separators.h"

static TreeNode* get_data(Tokens* tokens, NameTable* name_table, int* token_index, int* error);

static FrontendErrorCode split_to_words     (FrontendInputData* input_data);

static FrontendErrorCode check_type_arg     (char* str, TokenDataType* type_arg);
static FrontendErrorCode is_key_word        (char* str, int* is_id);
static FrontendErrorCode is_number          (char* str, int* is_num);
static FrontendErrorCode is_operator        (char* str, int* is_oper);
static FrontendErrorCode is_separator       (char* str, int* is_sep);
static FrontendErrorCode is_declarator      (char* str, int* is_dec);
static FrontendErrorCode is_identifier      (char* str, int* is_id);

static FrontendErrorCode write_token_value  (char* str, Token* token, NameTable* name_table);
static FrontendErrorCode write_key_word     (char* str, Token* token);
static FrontendErrorCode write_number       (char* str, Token* token);
static FrontendErrorCode write_operator     (char* str, Token* token);
static FrontendErrorCode write_separator    (char* str, Token* token);
static FrontendErrorCode write_declarator   (char* str, Token* token);
static FrontendErrorCode write_identifier   (char* str, Token* token, NameTable* name_table);

static FrontendErrorCode read_word          (char* buf, char* str);
static char* skip_spaces                    (char* str);
static char* skip_word                      (char* str);

static FrontendErrorCode file_to_buf        (const char* name_file, char** buf);
static FrontendErrorCode fsize              (const char* name_file, size_t* size_file);

FrontendErrorCode tree_input(Tree* tree, Tokens* tokens, NameTable* name_table)
{
    if (!tree) return FRONTEND_ERROR_TREE_NULL;

    int error = 0;
    int token_index = 0;
    tree->root = get_data(tokens, name_table, &token_index, &error);
    if (error) return error;

    return FRONTEND_ERROR_NO;
}

static TreeNode* get_data(Tokens* tokens, NameTable* name_table, int* token_index, int* error)
{
    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_init(FrontendInputData** input_data, char* name_file)
{
    FrontendErrorCode frontend_error = FRONTEND_ERROR_NO;

    *input_data = calloc(1, sizeof(FrontendInputData));
    if (!input_data) return FRONTEND_ERROR_ALLOC_FAIL;

    (*input_data)->count_words = 0;

    (*input_data)->name_file = strdup(name_file);
    if (!(*input_data)->name_file) return FRONTEND_ERROR_ALLOC_FAIL;

    frontend_error = file_to_buf((*input_data)->name_file, &(*input_data)->buf);
    if (frontend_error) return frontend_error;

    frontend_error = split_to_words(*input_data);
    if (frontend_error) return frontend_error;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_delete(FrontendInputData* input_data)
{
    if (!input_data) return FRONTEND_ERROR_NO;

    free(input_data->name_file);
    free(input_data->buf);

    for (size_t i = 0; i < input_data->count_words; i++)
        free(input_data->words[i]);
    input_data->count_words = 0;

    free(input_data);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode input_data_dump(FrontendInputData* input_data)
{
    fprintf(stderr, "Input Data:\n");
    fprintf(stderr, "file: %s\n", input_data->name_file);
    fprintf(stderr, "buf:\n%s\n", input_data->buf);

    fprintf(stderr, "-------------------------------\n");

    fprintf(stderr, "count words = %zu\n", input_data->count_words);
    for (size_t i = 0; i < input_data->count_words; i++) {
        fprintf(stderr, "word: %zu \t%s\n", i + 1, input_data->words[i]);
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode split_to_words(FrontendInputData* input_data)
{
    if (!input_data) return FRONTEND_ERROR_NO;
    
    FrontendErrorCode frontend_error = FRONTEND_ERROR_NO;

    char* buf = input_data->buf;

    input_data->count_words = 0;
    while (*buf != '\0') {
        if (input_data->count_words == MAX_COUNT_TOKENS) return FRONTEND_ERROR_MAX_COUNT_TOKENS;
        
        buf = skip_spaces(buf);

        char** word = &input_data->words[input_data->count_words];

        *word = calloc(1, MAX_SIZE_WORD);
        if (!*word) return FRONTEND_ERROR_ALLOC_FAIL;

        frontend_error = read_word(buf, *word);
        if (frontend_error) return frontend_error;

        buf = skip_word(buf);

        input_data->count_words++;

        buf = skip_spaces(buf);
    }

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tokenize(FrontendInputData* input_data, Tokens** tokens, NameTable* name_table)
{
    if (!input_data) return FRONTEND_ERROR_NO;
    
    FrontendErrorCode frontend_error = FRONTEND_ERROR_NO;

    frontend_error = tokens_init(tokens, input_data->count_words);
    if (frontend_error) return frontend_error;

    for (size_t i = 0; i < input_data->count_words; i++) {

        char* word = input_data->words[i];

        frontend_error = check_type_arg(word, &(*tokens)->token[i]->type);
        if (frontend_error) { 
            fprintf(stderr, print_lred("cannot read word \"%s\"\n"), word);
            return frontend_error;
        }

        frontend_error = write_token_value(word, (*tokens)->token[i], name_table);
        if (frontend_error) return frontend_error;
    }

    NameTableErrorCode name_table_error = name_table_resize_capacity(name_table, name_table->count);
    if (name_table_error) return FRONTEND_ERROR_NAME_TABLE;

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tokens_init(Tokens** tokens, size_t count_tokens)
{
    *tokens = calloc(1, sizeof(Tokens));
    if (!(*tokens)) return FRONTEND_ERROR_ALLOC_FAIL;

    (*tokens)->count_tokens = count_tokens;

    (*tokens)->token = calloc(count_tokens, sizeof(Token*));
    if (!(*tokens)->token) return FRONTEND_ERROR_ALLOC_FAIL;

    for (size_t i = 0; i < count_tokens; i++) {
        (*tokens)->token[i] = calloc(1, sizeof(Token));
        if (!(*tokens)->token[i]) return FRONTEND_ERROR_ALLOC_FAIL;
    }

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tokens_delete(Tokens* tokens)
{
    for (size_t i = 0; i < tokens->count_tokens; i++)
        free(tokens->token[i]);

    free(tokens->token);
    free(tokens);

    return FRONTEND_ERROR_NO;
}

FrontendErrorCode tokens_dump(Tokens* tokens)
{
    fprintf(stderr, "Tokens:\n");

    fprintf(stderr, "count tokens = %zu\n", tokens->count_tokens);
    for (size_t i = 0; i < tokens->count_tokens; i++) {
        fprintf(stderr, "token: %zu\t", i + 1);
        token_dump_cmd(tokens->token[i]);
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode check_type_arg(char* str, TokenDataType* type_arg)
{
    FrontendErrorCode error = FRONTEND_ERROR_NO;

    int is_kw = 0;
    error = is_key_word(str, &is_kw);
    if (error) return error;
    if (is_kw) {
        *type_arg = TYPE_KW;
        return FRONTEND_ERROR_NO;
    }

    int is_num = 0;
    error = is_number(str, &is_num);
    if (error) return error;
    if (is_num) {
        *type_arg = TYPE_NUM;
        return FRONTEND_ERROR_NO;
    }

    int is_oper = 0;
    error = is_operator(str, &is_oper);
    if (error) return error;
    if (is_oper) {
        *type_arg = TYPE_OP;
        return FRONTEND_ERROR_NO;
    }

    int is_sep = 0;
    error = is_separator(str, &is_sep);
    if (error) return error;
    if (is_sep) {
        *type_arg = TYPE_SEP;
        return FRONTEND_ERROR_NO;
    }

    int is_dec = 0;
    error = is_declarator(str, &is_dec);
    if (error) return error;
    if (is_dec) {
        *type_arg = TYPE_DEC;
        return FRONTEND_ERROR_NO;
    }

    int is_id = 0;
    error = is_identifier(str, &is_id);
    if (error) return error;
    if (is_id) {
        *type_arg = TYPE_ID;
        return FRONTEND_ERROR_NO;
    }

    *type_arg = TYPE_ERR;
    return FRONTEND_ERROR_INVALID_WORD;
}

static FrontendErrorCode is_key_word(char* str, int* is_kw)
{
    for (int i = 0; i < COUNT_KEY_WORDS; i++) {
        if (strcmp(KEY_WORDS[i].name, str) == 0) {
            *is_kw = 1;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode is_number(char* str, int* is_num)
{
    int i = 0;
    int is_minus = 0;

    if (str[i] == '-') {
        i++;
        is_minus = 1;
    }

    while (str[i] != '\0' && !isspace(str[i])) {

        if (str[i] < '0' || str[i] > '9') {
            *is_num = 0;
            return FRONTEND_ERROR_NO;
        }

        i++;
    }
    if (i > 1 || (i == 1 && !is_minus)) *is_num = 1;

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode is_operator(char* str, int* is_oper)
{
    for (int i = 0; i < COUNT_OPERATORS; i++) {
        if (strcmp(OPERATORS[i].name, str) == 0) {
            *is_oper = 1;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode is_separator(char* str, int* is_sep)
{
    for (int i = 0; i < COUNT_SEPARATORS; i++) {
        if (strcmp(SEPARATORS[i].name, str) == 0) {
            *is_sep = 1;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode is_declarator(char* str, int* is_dec)
{
    for (int i = 0; i < COUNT_DECLARATORS; i++) {
        if (strcmp(DECLARATORS[i].name, str) == 0) {
            *is_dec = 1;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode is_identifier(char* str, int* is_id)
{
    int i = 0;
    while (str[i] != '\0' && !isspace(str[i])) {

        if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || str[i] == '_'))
            break;

        i++;
    }

    while (str[i] != '\0' && !isspace(str[i])) {

        if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') ||
              str[i] == '_' || (str[i] >= '0' && str[i] <= '9'))) {

            *is_id = 0;
            return FRONTEND_ERROR_READ_FILE;
        }

        i++;
    }

    *is_id = 1;
    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_token_value(char* str, Token* token, NameTable* name_table)
{
    FrontendErrorCode error = FRONTEND_ERROR_NO;

    if (token->type == TYPE_KW)  error = write_key_word  (str, token);
    if (token->type == TYPE_NUM) error = write_number    (str, token);
    if (token->type == TYPE_OP)  error = write_operator  (str, token);
    if (token->type == TYPE_SEP) error = write_separator (str, token);
    if (token->type == TYPE_DEC) error = write_declarator(str, token);
    if (token->type == TYPE_ID)  error = write_identifier(str, token, name_table);

    if (error) return error;

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_key_word(char* str, Token* token)
{
    for (int i = 0; i < COUNT_KEY_WORDS; i++) {
        if (strcmp(KEY_WORDS[i].name, str) == 0) {
            token->value = KEY_WORDS[i].type_key_word;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_number(char* str, Token* token)
{
    token->value = atoi(str);

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_operator(char* str, Token* token)
{
    for (int i = 0; i < COUNT_OPERATORS; i++) {
        if (strcmp(OPERATORS[i].name, str) == 0) {
            token->value = OPERATORS[i].type_operator;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_separator(char* str, Token* token)
{
    for (int i = 0; i < COUNT_SEPARATORS; i++) {
        if (strcmp(SEPARATORS[i].name, str) == 0) {
            token->value = SEPARATORS[i].type_separator;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_declarator(char* str, Token* token)
{
    for (int i = 0; i < COUNT_DECLARATORS; i++) {
        if (strcmp(DECLARATORS[i].name, str) == 0) {
            token->value = DECLARATORS[i].type_declarator;
            break;
        }
    }

    return FRONTEND_ERROR_NO;
}

static FrontendErrorCode write_identifier(char* str, Token* token, NameTable* name_table)
{
    int was = 0;
    for (size_t i = 0; i < name_table->count; i++) {
        if (strcmp(name_table->name[i], str) == 0) {
            token->value = (int)i;
            was = 1;
            break;
        }
    }

    if (!was) {
        token->value = (int)name_table->count;
        name_table->name[name_table->count] = strdup(str);
        if (!name_table->name[name_table->count]) return FRONTEND_ERROR_STRDUP;

        name_table->count++;
        if (name_table->count >= name_table->capacity - 1) {
            int error = name_table_increase(name_table);
            if (error) return error;
        }
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