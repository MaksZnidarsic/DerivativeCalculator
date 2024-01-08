


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum TokenType {
    TokenTypeUnknown, TokenTypeEndOfInput,
    TokenTypeDependent, TokenTypeConstant, TokenTypeNumber,
    TokenTypePlus, TokenTypeMinus, TokenTypeTimes, TokenTypeSlash, TokenTypeExponent,
    TokenTypeLParen, TokenTypeRParen,

    TokenTypeFunction,
    TokenTypeAbs, TokenTypeSqrt, TokenTypeLn,
    TokenTypeSin, TokenTypeCos, TokenTypeTan, TokenTypeCot, TokenTypeCsc, TokenTypeSec,
    TokenTypeArcsin, TokenTypeArccos, TokenTypeArctan, TokenTypeArccot, TokenTypeArcsec, TokenTypeArccsc,
};


typedef struct Input Input;
typedef struct Token Token;

struct Token {
    enum TokenType type;
    char constant;
    int num;
};

struct Input {
    const char* input; //cleaned outside
    unsigned int len;
    unsigned int pos;
    char var;
};

Input* createInput(const char* text, char variable);
Token* inputNext(Input* input);
void* cleanInput(Input* input);

Token* createToken(enum TokenType type, char constant, double num);
void* cleanToken(Token* token);

void printTokenString(enum TokenType token);
void printToken(Token* token);





