


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


Token* inputReadNumber(Input* input) {
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


Token* inputNext(Input* input) {
    Token* token = NULL;
    if (input->pos >= input->len) {
        token = createToken(TokenTypeEndOfInput, 0, 0);
        return token;
    }
    
    inputSkipWhitespace(input);
    switch (input->input[input->pos]) {
        case '+': token = createToken(TokenTypePlus, 0, 0); break;
        case '-': token = createToken(TokenTypeMinus, 0, 0); break;
        case '*': token = createToken(TokenTypeTimes, 0, 0); break;
        case '/': token = createToken(TokenTypeSlash, 0, 0); break;
        case '^': token = createToken(TokenTypeExponent, 0, 0); break;
        case '(': token = createToken(TokenTypeLParen, 0, 0); break;
        case ')': token = createToken(TokenTypeRParen, 0, 0); break;
        case 'a':
            if (strstr(input->input + input->pos, "abs") == input->input + input->pos) {
                token = createToken(TokenTypeAbs, 0, 0);
                input->pos += 2; break;
            }
            if (strstr(input->input + input->pos, "arcsin") == input->input + input->pos) {
                token = createToken(TokenTypeArcsin, 0, 0);
                input->pos += 5; break;
            }
            if (strstr(input->input + input->pos, "arccos") == input->input + input->pos) {
                token = createToken(TokenTypeArccos, 0, 0);
                input->pos += 5; break;
            }
            if (strstr(input->input + input->pos, "arctan") == input->input + input->pos) {
                token = createToken(TokenTypeArctan, 0, 0);
                input->pos += 5; break;
            }
            if (strstr(input->input + input->pos, "arccot") == input->input + input->pos) {
                token = createToken(TokenTypeArccot, 0, 0);
                input->pos += 5; break;
            }
            if (strstr(input->input + input->pos, "arcsec") == input->input + input->pos) {
                token = createToken(TokenTypeArcsec, 0, 0);
                input->pos += 5; break;
            }
            if (strstr(input->input + input->pos, "arccsc") == input->input + input->pos) {
                token = createToken(TokenTypeArccsc, 0, 0);
                input->pos += 5; break;
            }
        case 'c':
            if (strstr(input->input + input->pos, "cos") == input->input + input->pos) {
                token = createToken(TokenTypeCos, 0, 0);
                input->pos += 2; break;
            }
            if (strstr(input->input + input->pos, "cot") == input->input + input->pos) {
                token = createToken(TokenTypeCot, 0, 0);
                input->pos += 2; break;
            }
            if (strstr(input->input + input->pos, "csc") == input->input + input->pos) {
                token = createToken(TokenTypeCsc, 0, 0);
                input->pos += 2; break;
            }
        case 'l':
            if (strstr(input->input + input->pos, "ln") == input->input + input->pos) {
                token = createToken(TokenTypeLn, 0, 0);
                input->pos++; break;
            }
        case 's':
            if (strstr(input->input + input->pos, "sec") == input->input + input->pos) {
                token = createToken(TokenTypeSec, 0, 0);
                input->pos += 2; break;
            }
            if (strstr(input->input + input->pos, "sin") == input->input + input->pos) {
                token = createToken(TokenTypeSin, 0, 0);
                input->pos += 2; break;
            }
            if (strstr(input->input + input->pos, "sqrt") == input->input + input->pos) {
                token = createToken(TokenTypeSqrt, 0, 0);
                input->pos += 3; break;
            }
        case 't':
            if (strstr(input->input + input->pos, "tan") == input->input + input->pos) {
                token = createToken(TokenTypeTan, 0, 0);
                input->pos += 2;break;
            }
    }
    if (!token) {
        if (isDigit(input->input[input->pos]))
            return inputReadNumber(input);
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


void printTokenString(enum TokenType token) {
    switch (token) {
        case TokenTypeUnknown: printf("Unknown"); return;
        case TokenTypeEndOfInput: printf("EndOfInput"); return;
        case TokenTypeDependent: printf("Variable"); return;
        case TokenTypeConstant: printf("Constant"); return;
        case TokenTypeNumber: printf("Number"); return;
        case TokenTypePlus: printf("+"); return;
        case TokenTypeMinus: printf("-"); return;
        case TokenTypeTimes: printf("*"); return;
        case TokenTypeSlash: printf("/"); return;
        case TokenTypeExponent: printf("^"); return;
        case TokenTypeLParen: printf("("); return;
        case TokenTypeRParen: printf(")"); return;
        case TokenTypeFunction: return;
        case TokenTypeAbs: printf("abs"); return;
        case TokenTypeSqrt: printf("sqrt"); return;
        case TokenTypeLn: printf("ln"); return;
        case TokenTypeSin: printf("sin"); return;
        case TokenTypeCos: printf("cos"); return;
        case TokenTypeTan: printf("tan"); return;
        case TokenTypeCot: printf("cot"); return;
        case TokenTypeSec: printf("sec"); return;
        case TokenTypeCsc: printf("csc"); return;
        case TokenTypeArcsin: printf("arcsin"); return;
        case TokenTypeArccos: printf("arccos"); return;
        case TokenTypeArctan: printf("arctan"); return;
        case TokenTypeArccot: printf("arccot"); return;
    }
    return;
}


void printToken(Token* token) {
    if (!token) return;
    if (token->type == TokenTypeConstant || token->type == TokenTypeDependent) {
        printTokenString(token->type); printf("[%c]", token->constant); return;
    }
    if (token->type == TokenTypeNumber) {
        printTokenString(token->type); printf("[%d]", token->num); return;
    }
    printTokenString(token->type);
}





