#ifndef FRONTEND_KEY_WORDS_H
#define FRONTEND_KEY_WORDS_H

typedef enum _TypeKeyWord {
    KW_ERR =    0,
    KW_IF =     1,
    KW_ELSE =   2,
    KW_WHILE =  3,
    KW_RETURN = 4
} TypeKeyWord;

typedef struct _KeyWord {
    TypeKeyWord type_key_word;
    const char* name;
} KeyWord;

static const KeyWord KEY_WORDS[] = {
    {KW_ERR,    "error_key_word"},
    {KW_IF,     "if"},
    {KW_ELSE,   "else"},
    {KW_WHILE,  "while"},
    {KW_RETURN, "return"}
};
static const int COUNT_KEY_WORDS = sizeof(KEY_WORDS) / sizeof(KeyWord);

#endif // FRONTEND_KEY_WORDS_H