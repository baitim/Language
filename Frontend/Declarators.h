#ifndef FRONTEND_DECLARATORS_H
#define FRONTEND_DECLARATORS_H

typedef enum _TypeDeclarator {
    DEC_ERR =    0,
    DEC_VAR =    1,
    DEC_FUNC =   2,
} TypeDeclarator;

typedef struct _Declarator {
    TypeDeclarator type_declarator;
    const char* name;
} Declarator;

static const Declarator DECLARATORS[] = {
    {DEC_ERR,    "error_declarator"},
    {DEC_VAR,    "var"},
    {DEC_FUNC,   "func"},
};
static const int COUNT_DECLARATORS = sizeof(DECLARATORS) / sizeof(Declarator);

#endif // FRONTEND_DECLARATORS_H