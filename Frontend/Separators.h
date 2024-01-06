#ifndef FRONTEND_SEPARATORS_H
#define FRONTEND_SEPARATORS_H

typedef enum _TypeSeparator {
    SEP_ERR =        0,
    SEP_EXP_LEFT =  -1,
    SEP_EXP_RIGHT =  1,
    SEP_STAT_LEFT = -2,
    SEP_STAT_RIGHT = 2,
    SEP_PAR_LEFT =  -3,
    SEP_PAR_RIGHT =  3,
    SEP_END_LINE =   4,
    SEP_END_STAT =   5
} TypeSeparator;

typedef struct _Separator {
    TypeSeparator type_separator;
    const char* name;
} Separator;

static const Separator SEPARATORS[] = {
    {SEP_ERR,           "error_separator"},
    {SEP_EXP_LEFT,      "("},
    {SEP_EXP_RIGHT,     ")"},
    {SEP_STAT_LEFT,     "{"},
    {SEP_STAT_RIGHT,    "}"},
    {SEP_PAR_LEFT,      "<"},
    {SEP_PAR_RIGHT,     ">"},
    {SEP_END_LINE,      "end"},
    {SEP_END_STAT,      ":"}
};
static const int COUNT_SEPARATORS = sizeof(SEPARATORS) / sizeof(Separator);

#endif // FRONTEND_SEPARATORS_H