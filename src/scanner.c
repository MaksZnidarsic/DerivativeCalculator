


#include "scanner.h"


Token makeToken(TokenType type, const char** pos, unsigned int len) {
    Token token;
    token.type = type;
    token.lit = *pos;
    token.len = len;
    (*pos) += len;
    return token;
}

void skipWhitespace(const char** pos) {
    while (**pos == ' ' || **pos == '\n' || **pos == '\t' || **pos == '\r') (*pos)++;
}

unsigned int isDigit(char c) {
    return c >= '0' && c <= '9';
}

Token number(const char** pos) {
    unsigned int len = 1;
    while (isDigit(*(*pos + len))) len++;
    return makeToken(TokenNum, pos, len);
}

Token checkKeyword(const char** pos) {
    switch (**pos) {
        case 'c': switch (*(*pos + 1)) {
            case 'o': switch (*(*pos + 2)) {
                case 's': return makeToken(TokenCos, pos, 3);
                case 't': return makeToken(TokenCot, pos, 3);
            } break;
            case 's': switch (*(*pos + 2)) {
                case 'c': return makeToken(TokenCsc, pos, 3);
            } break;
        } break;
        case 'l': switch (*(*pos + 1)) {
            case 'n': return makeToken(TokenLn, pos, 2);
        } break;
        case 's': switch (*(*pos + 1)) {
            case 'e': switch (*(*pos + 2)) {
                case 'c': return makeToken(TokenSec, pos, 3);
            } break;
            case 'i': switch (*(*pos + 2)) {
                case 'n': return makeToken(TokenSin, pos, 3);
            } break;
        } break;
        case 't': switch (*(*pos + 1)) {
            case 'a': switch (*(*pos + 2)) {
                case 'n': return makeToken(TokenTan, pos, 3);
            } break;
        } break;
        /* case '\\': Add LaTeX support. */
    }
    return makeToken(TokenVar, pos, 1);
}

Token scan(const char** pos) {
    skipWhitespace(pos);
    if (**pos == 0x0) return makeToken(EndOfInput, pos, 0);
    switch (**pos) {
        case '(': return makeToken(TokenLParen, pos, 1);
        case ')': return makeToken(TokenRParen, pos, 1);
        case '^': return makeToken(TokenExp, pos, 1);
        case '-': return makeToken(TokenMinus, pos, 1);
        case '+': return makeToken(TokenPlus, pos, 1);
        case '/': return makeToken(TokenSlash, pos, 1);
        case '*': return makeToken(TokenStar, pos, 1);
    }
    if (isDigit(**pos)) return number(pos);
    return checkKeyword(pos);
}
