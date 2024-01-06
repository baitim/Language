#ifndef FRONTEND_OPERATORS_H
#define FRONTEND_OPERATORS_H

typedef enum _TypeOperator {
    OP_ERR =        0,
    OP_ASS =        1,
    OP_ADD =        2,
    OP_SUB =        3,
    OP_MUL =        4,
    OP_DIV =        5,
    OP_POW =        6,
    OP_LESS    =    7,
    OP_LESS_EQ =    8,
    OP_EQUAL   =    9,
    OP_UNEQUAL =    10,
    OP_MORE_EQ =    11,
    OP_MORE    =    12
} TypeOperator;

typedef struct _Operator {
    TypeOperator type_operator;
    const char* name;
} Operator;

static const Operator OPERATORS[] = {
    {OP_ERR,        "error_operator"},
    {OP_ASS,        "assign"},
    {OP_ADD,        "plus"},
    {OP_SUB,        "minus"},
    {OP_MUL,        "multiply"},
    {OP_DIV,        "divide"},
    {OP_POW,        "**"},
    {OP_LESS,       "less"},
    {OP_LESS_EQ,    "less_equal"},
    {OP_EQUAL,      "equal"},
    {OP_UNEQUAL,    "unequal"},
    {OP_MORE_EQ,    "more_equal"},
    {OP_MORE,       "more"}
};
static const int COUNT_OPERATORS = sizeof(OPERATORS) / sizeof(Operator);

#endif // FRONTEND_OPERATORS_H