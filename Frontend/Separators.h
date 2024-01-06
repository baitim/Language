#ifndef FRONTEND_SEPARATORS_H
#define FRONTEND_SEPARATORS_H

typedef enum _TypeSeparator {
    SEP_ERR =        0,
    SEP_PAR_LEFT =  -1,
    SEP_PAR_RIGHT =  1,
} TypeSeparator;

typedef struct _Separator {
    TypeSeparator type_separator;
    const char* name;
} Separator;

static const Separator SEPARATORS[] = {
    {SEP_ERR,           "error_parenthesis"},
    {SEP_PAR_LEFT,      "left"},
    {SEP_PAR_RIGHT,     "right"},
};
static const int COUNT_SEPARATORS = sizeof(SEPARATORS) / sizeof(Separator);

#endif // FRONTEND_SEPARATORS_H