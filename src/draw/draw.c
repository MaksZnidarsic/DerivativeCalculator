


#include "draw.h"


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
        case TokenTypeLog: printf("log"); return;
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
        case TokenTypeArcsec: printf("arcsec"); return;
        case TokenTypeArccsc: printf("arccsc"); return;
        case TokenTypeSinh: printf("sinh"); return;
        case TokenTypeCosh: printf("cosh"); return;
        case TokenTypeTanh: printf("tanh"); return;
        case TokenTypeCoth: printf("coth"); return;
        case TokenTypeSech: printf("sech"); return;
        case TokenTypeCsch: printf("csch"); return;
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


void printTree(void* expression, unsigned int indent) {
    if (!expression) return;
    for (unsigned int i = 0; i < indent; i++)
        printf("|   ");
    switch (*(int*)expression) {
        case ExpressionTypeBinary:
            printf("Bi "); printTokenString(((BinaryExpression*)expression)->op); printf("\n");
            printTree(((BinaryExpression*)expression)->left, indent + 1);
            printTree(((BinaryExpression*)expression)->right, indent + 1); break;
        case ExpressionTypeUnary:
            printf("Un "); printTokenString(((UnaryExpression*)expression)->op); printf("\n");
            printTree(((UnaryExpression*)expression)->left, indent + 1); break;
        case ExpressionTypeConstant:
            printf("Cs %c\n", ((VariableExpression*)expression)->ch); break;
        case ExpressionTypeDependent:
            printf("Dp %c\n", ((VariableExpression*)expression)->ch); break;
        case ExpressionTypeNumber:
            printf("Nm %d\n", ((NumberExpression*)expression)->num); break;
    }
}


unsigned int checkCommutative(enum TokenType op) {
    switch ((int)op) {
        case TokenTypePlus:
        case TokenTypeTimes:
            return 1;
    }
    return 0;
}


void printExpression(void* expression) {
    if (!expression) return;
    switch (*(int*)expression) {
        case ExpressionTypeBinary:
            if (((BinaryExpression*)expression)->op == TokenTypeExponent
                    && *(int*)((BinaryExpression*)expression)->left == ExpressionTypeUnary) {
                printf("("); printExpression(((BinaryExpression*)expression)->left); printf(")");
            }
            else if (*(int*)((BinaryExpression*)expression)->left == ExpressionTypeBinary
                    && getPrecedence(((BinaryExpression*)((BinaryExpression*)expression)->left)->op) < getPrecedence(((BinaryExpression*)expression)->op)) {
                printf("("); printExpression(((BinaryExpression*)expression)->left); printf(")");
            }
            else printExpression(((BinaryExpression*)expression)->left);
            if (((BinaryExpression*)expression)->op != TokenTypeTimes)
                printTokenString(((BinaryExpression*)expression)->op);
            if (*(int*)((BinaryExpression*)expression)->right == ExpressionTypeBinary
                    && getPrecedence(((BinaryExpression*)((BinaryExpression*)expression)->right)->op) == getPrecedence(((BinaryExpression*)expression)->op)
                    && !checkCommutative(((BinaryExpression*)expression)->op)) {
                printf("("); printExpression(((BinaryExpression*)expression)->right); printf(")");
            }
            else if (*(int*)((BinaryExpression*)expression)->right == ExpressionTypeBinary
                    && getPrecedence(((BinaryExpression*)((BinaryExpression*)expression)->right)->op) < getPrecedence(((BinaryExpression*)expression)->op)) {
                printf("("); printExpression(((BinaryExpression*)expression)->right); printf(")");
            }
            else printExpression(((BinaryExpression*)expression)->right); break;
        case ExpressionTypeUnary:
            printTokenString(((UnaryExpression*)expression)->op);
            if (*(int*)((UnaryExpression*)expression)->left == ExpressionTypeDependent
                    || *(int*)((UnaryExpression*)expression)->left == ExpressionTypeConstant) {
                printExpression(((UnaryExpression*)expression)->left); break;
            }
            else {
                printf("("); printExpression(((UnaryExpression*)expression)->left); printf(")"); break;
            }
        case ExpressionTypeConstant:
            printf("%c", ((VariableExpression*)expression)->ch); break;
        case ExpressionTypeDependent:
            printf("%c", ((VariableExpression*)expression)->ch); break;
        case ExpressionTypeNumber:
            printf("%d", ((NumberExpression*)expression)->num); break;
    }
}


void printList(void* list) {
    if (!list) return;
    for (unsigned int i = 0; i < ((List*)list)->len; i++) {
        printf("0x%p | ", ((List*)list)->data[i]);
        switch (*(int*)((List*)list)->data[i]) {
            case ExpressionTypeBinary: printf("Bi"); break;
            case ExpressionTypeUnary: printf("Un"); break;
            case ExpressionTypeConstant: printf("Cs"); break;
            case ExpressionTypeDependent: printf("Dp"); break;
            case ExpressionTypeNumber: printf("Nm"); break;
        }
        printf(" | ");
        printExpression(((List*)list)->data[i]);
        printf("\n");
    }
}





