


#include "token.h"


void inputRead(Input* input) {
    if (input->pos >= input->len) return;
    input->pos++;
}


void inputSkipWhitespace(Input* input) {
    while (input->input[input->pos] == ' ' || input->input[input->pos] == '\t' || input->input[input->pos] == '\n' || input->input[input->pos] == '\r')
        inputRead(input);
}


Input* createInput(const char* text, char variable) {
    if (!text)
        return NULL;
    Input* input = malloc(sizeof(Input));
    input->input = text;
    input->len = strlen(text);
    input->pos = 0;
    input->var = variable;
    return input;
}


unsigned int isDigit(char ch) {
    if (ch >= '0' && ch <= '9')
        return 1;
    return 0;
}


Token* readNumber(Input* input) {
    unsigned int pos = input->pos;
    while (isDigit(input->input[input->pos]))
        inputRead(input);
    unsigned int len = input->pos - pos;
    char* numstr = malloc(len + 1);
    memcpy(numstr, input->input + pos, len);
    numstr[len] = 0x0;
    int num = atoi(numstr);
    free(numstr);
    return createToken(TokenTypeNumber, 0, num);
}


Token* readKeyword(Input* input, char a) {
    //implement trie
    switch (a) {
        case 'a':
            if (strstr(input->input + input->pos, "abs") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeAbs, 0, 0);
            }
            if (strstr(input->input + input->pos, "arcsin") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArcsin, 0, 0);
            }
            if (strstr(input->input + input->pos, "arccos") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArccos, 0, 0);
            }
            if (strstr(input->input + input->pos, "arctan") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArctan, 0, 0);
            }
            if (strstr(input->input + input->pos, "arccot") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArccot, 0, 0);
            }
            if (strstr(input->input + input->pos, "arcsec") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArcsec, 0, 0);
            }
            if (strstr(input->input + input->pos, "arccsc") == input->input + input->pos) {
                input->pos += 5;
                return createToken(TokenTypeArccsc, 0, 0);
            }
        case 'c':
            if (strstr(input->input + input->pos, "cosh") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeCosh, 0, 0);
            }
            if (strstr(input->input + input->pos, "cos") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeCos, 0, 0);
            }
            if (strstr(input->input + input->pos, "coth") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeCoth, 0, 0);
            }
            if (strstr(input->input + input->pos, "cot") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeCot, 0, 0);
            }
            if (strstr(input->input + input->pos, "csch") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeCsch, 0, 0);
            }
            if (strstr(input->input + input->pos, "csc") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeCsc, 0, 0);
            }
        case 'l':
            if (strstr(input->input + input->pos, "ln") == input->input + input->pos) {
                input->pos++;
                return createToken(TokenTypeLn, 0, 0);
            }
            if (strstr(input->input + input->pos, "log") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeLog, 0, 0);
            }
        case 's':
            if (strstr(input->input + input->pos, "sech") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeSec, 0, 0);
            }
            if (strstr(input->input + input->pos, "sec") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeSec, 0, 0);
            }
            if (strstr(input->input + input->pos, "sinh") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeSinh, 0, 0);
            }
            if (strstr(input->input + input->pos, "sin") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeSin, 0, 0);
            }
            if (strstr(input->input + input->pos, "sqrt") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeSqrt, 0, 0);
            }
        case 't':
            if (strstr(input->input + input->pos, "tanh") == input->input + input->pos) {
                input->pos += 3;
                return createToken(TokenTypeTanh, 0, 0);
            }
            if (strstr(input->input + input->pos, "tan") == input->input + input->pos) {
                input->pos += 2;
                return createToken(TokenTypeTan, 0, 0);
            } 
        }
    return NULL;
}


Token* inputNext(Input* input) {
    Token* token = NULL;
    if (input->pos >= input->len)
        return createToken(TokenTypeEndOfInput, 0, 0);
    
    inputSkipWhitespace(input);
    switch (input->input[input->pos]) {
        case '+': token = createToken(TokenTypePlus, 0, 0); break;
        case '-': token = createToken(TokenTypeMinus, 0, 0); break;
        case '*': token = createToken(TokenTypeTimes, 0, 0); break;
        case '/': token = createToken(TokenTypeSlash, 0, 0); break;
        case '^': token = createToken(TokenTypeExponent, 0, 0); break;
        case '(': token = createToken(TokenTypeLParen, 0, 0); break;
        case ')': token = createToken(TokenTypeRParen, 0, 0); break;
        default: token = readKeyword(input, input->input[input->pos]);
    }
    if (!token) {
        if (isDigit(input->input[input->pos]))
            return readNumber(input);
        if (input->input[input->pos] == input->var)
            token = createToken(TokenTypeDependent, 0, 0);
        else
            token = createToken(TokenTypeConstant, input->input[input->pos], 0);
    }

    inputRead(input);
    return token;
}


void* cleanInput(Input* input) {
    if (!input)
        return NULL;
    free(input);
    return NULL;
}


Token* createToken(enum TokenType type, char constant, double num) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->constant = constant;
    token->num = num;
    return token;
}


void* cleanToken(Token* token) {
    if (!token)
        return NULL;
    free(token);
    return NULL;
}








