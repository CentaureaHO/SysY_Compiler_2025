#ifndef __TOKEN_H__
#define __TOKEN_H__

typedef union
{
    int   int_val;
    float float_val;
    char* str_val;
} YYSTYPE;

#define DECLARE               \
    X(IDENT, 17171017)        \
                              \
    X(INT_CONST, 1)           \
    X(FLOAT_CONST, 2)         \
    X(STR_CONST, 3)           \
                              \
    X(INT, 4)                 \
    X(FLOAT, 5)               \
    X(VOID, 6)                \
                              \
    X(IF, 7)                  \
    X(ELSE, 8)                \
    X(FOR, 9)                 \
    X(WHILE, 10)              \
    X(CONTINUE, 11)           \
    X(BREAK, 12)              \
    X(SWITCH, 13)             \
    X(CASE, 14)               \
    X(GOTO, 15)               \
    X(DO, 16)                 \
    X(RETURN, 17)             \
                              \
    X(CONST, 18)              \
                              \
    X(SEMICOLON, 19) /* ; */  \
    X(COMMA, 20)     /* , */  \
    X(ASSIGN, 21)    /* = */  \
    X(NOT, 22)       /* ! */  \
    X(BITOR, 23)     /* | */  \
    X(BITAND, 24)    /* & */  \
    X(GT, 25)        /* > */  \
    X(LT, 26)        /* < */  \
    X(PLUS, 27)      /* + */  \
    X(MINUS, 28)     /* - */  \
    X(STAR, 29)      /* * */  \
    X(SLASH, 30)     /* / */  \
    X(DOT, 31)       /* . */  \
    X(GE, 32)        /* >= */ \
    X(LE, 33)        /* <= */ \
    X(EQ, 34)        /* == */ \
                              \
    X(LPAREN, 35)   /* ( */   \
    X(RPAREN, 36)   /* ) */   \
    X(LBRACKET, 37) /* [ */   \
    X(RBRACKET, 38) /* ] */   \
    X(LBRACE, 39)   /* { */   \
    X(RBRACE, 40)   /* } */

enum TokenType
{
#define X(a, b) a = b,
    DECLARE
#undef X
};

const char* getName(int token);

#endif