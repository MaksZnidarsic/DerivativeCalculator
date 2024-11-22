


#pragma once


typedef enum TokenType {
    EndOfInput,
    TokenLParen, TokenRParen,
    TokenNum, TokenVar,
    TokenExp, TokenMinus, TokenPlus, TokenSlash, TokenStar,
    TokenCos, TokenCot, TokenCsc, TokenLn, TokenSec, TokenSin, TokenTan,
} TokenType;


typedef struct Token {
    TokenType type;
    const char* lit;
    unsigned int len;
} Token;

Token scan(const char** pos); //const char** pos in order to keep state outside.
