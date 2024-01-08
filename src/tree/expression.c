


#include "token.h"
#include "tree.h"


void orderBinaryExpression(BinaryExpression* expression) {
    if (expression->op != TokenTypeTimes && expression->op != TokenTypePlus) return;
    if (*(int*)expression->left > *(int*)expression->right) return;
    if (*(int*)expression->left == *(int*)expression->right)
        switch (*(int*)expression->left) {
            case ExpressionTypeBinary:
                if (((BinaryExpression*)expression->left)->op <= ((BinaryExpression*)expression->right)->op) return;
                break;
            case ExpressionTypeUnary:
                if (((UnaryExpression*)expression->left)->op <= ((UnaryExpression*)expression->right)->op) return;
                break;
            case ExpressionTypeDependent: return;
            case ExpressionTypeConstant:
                if (((VariableExpression*)expression->left)->ch >= ((VariableExpression*)expression->right)->ch) return;
                break;
            case ExpressionTypeNumber:
                if (((NumberExpression*)expression->left)->num >= ((NumberExpression*)expression->right)->num) return;
                break;
        }
    void* temp = expression->left;
    expression->left = expression->right;
    expression->right = temp;
}


BinaryExpression* createBinaryExpression(List* list, enum TokenType op, void* left, void* right) {
    BinaryExpression* expression = malloc(sizeof(BinaryExpression));
    expression->type = ExpressionTypeBinary;
    expression->op = op;
    expression->left = left;
    expression->right = right;
    orderBinaryExpression(expression);
    listPushExpression(list, expression);
    return expression;
}


UnaryExpression* createUnaryExpression(List* list, enum TokenType op, void* left) {
    UnaryExpression* expression = malloc(sizeof(UnaryExpression));
    expression->type = ExpressionTypeUnary;
    expression->op = op;
    expression->left = left;
    listPushExpression(list, expression);
    return expression;
}


VariableExpression* createDependentExpression(List* list, char ch) {
    VariableExpression* expression = malloc(sizeof(VariableExpression));
    expression->type = ExpressionTypeDependent;
    expression->ch = ch;
    listPushExpression(list, expression);
    return expression;
}


VariableExpression* createConstantExpression(List* list, char ch) {
    VariableExpression* expression = malloc(sizeof(VariableExpression));
    expression->type = ExpressionTypeConstant;
    expression->ch = ch;
    listPushExpression(list, expression);
    return expression;
}


NumberExpression* createNumberExpression(List* list, double num) {
    NumberExpression* expression = malloc(sizeof(NumberExpression));
    expression->type = ExpressionTypeNumber;
    expression->num = num;
    listPushExpression(list, expression);
    return expression;
}


enum OperatorPrecedence getPrecedence(enum TokenType token) {
    switch (token) {
        case TokenTypeRParen: return OperatorPrecedenceLowest;
        case TokenTypePlus:
        case TokenTypeMinus:
            return OperatorPrecedenceSum;
        case TokenTypeTimes:
        case TokenTypeSlash:
            return OperatorPrecedenceProduct;
        case TokenTypeExponent: return OperatorPrecedenceExponent;
        default:
            return OperatorPrecedenceProduct;
    }
}


VariableExpression* parseConstantExpression(List* list, Parser* parser, Input* input) {
    VariableExpression* expression = createConstantExpression(list, parser->curr->constant);
    parserNext(parser, input);
    return expression;
}


VariableExpression* parseDependentExpression(List* list, Parser* parser, Input* input) {
    VariableExpression* expression = createDependentExpression(list, input->var);
    parserNext(parser, input);
    return expression;
}


NumberExpression* parseNumberExpression(List* list, Parser* parser, Input* input) {
    NumberExpression* expression = createNumberExpression(list, parser->curr->num);
    parserNext(parser, input);
    return expression;
}


void* parseGroupedExpression(List* list, Parser* parser, Input* input) {
    parserNext(parser, input);
    void* expression = parseExpression(list, parser, input, OperatorPrecedenceLowest);
    parserNext(parser, input);
    return expression;
}


void* parseUnaryExpression(List* list, Parser* parser, Input* input);

void* parseArgument(List* list, Parser* parser, Input* input) {
    switch ((int)parser->curr->type) {
        case TokenTypeLParen: return parseGroupedExpression(list, parser, input);
        case TokenTypeDependent: return parseDependentExpression(list, parser, input);
        case TokenTypeConstant: return parseConstantExpression(list, parser, input);
        case TokenTypeNumber: return parseNumberExpression(list, parser, input);
        case TokenTypeMinus: return parseUnaryExpression(list, parser, input);
    }
    if (parser->curr->type > TokenTypeFunction)
        return parseUnaryExpression(list, parser, input);
    parser->error++;
    return NULL;
}


void* parseUnaryExpression(List* list, Parser* parser, Input* input) {
    enum TokenType op = parser->curr->type;
    parserNext(parser, input);
    if (parser->curr->type == TokenTypeExponent) {
        parserNext(parser, input);
        void* right = parseArgument(list, parser, input);
        if (parserCheckNull(parser, right))
            return NULL;
        void* argleft = parseArgument(list, parser, input);
        if (parserCheckNull(parser, argleft))
            return NULL;
        void* left = createUnaryExpression(list, op, argleft);
        return createBinaryExpression(list, TokenTypeExponent, left, right);
    }
    void* left = parseArgument(list, parser, input);
    if (parserCheckNull(parser, left))
        return NULL;
    return createUnaryExpression(list, op, left);
}


BinaryExpression* parseBinaryExpression(List* list, Parser* parser, Input* input, void* left) {
    enum TokenType op = parser->curr->type;
    enum OperatorPrecedence precedence = getPrecedence(parser->curr->type);
    parserNext(parser, input);
    void* right = parseExpression(list, parser, input, precedence);
    if (parserCheckNull(parser, right))
        return NULL;
    return createBinaryExpression(list, op, left, right);
}


BinaryExpression* parseNoTimes(List* list, Parser* parser, Input* input, void* left) {
    void* right = parseExpression(list, parser, input, OperatorPrecedenceProduct);
    if (parserCheckNull(parser, right))
        return NULL;
    return createBinaryExpression(list, TokenTypeTimes, left, right);
}


void* parseExpression(List* list, Parser* parser, Input* input, enum OperatorPrecedence precedence) {
    void* left;
    enum TokenType token = parser->curr->type;
    switch ((int)token) {
        case TokenTypeConstant: left = parseConstantExpression(list, parser, input); break;
        case TokenTypeDependent: left = parseDependentExpression(list, parser, input); break;
        case TokenTypeNumber: left = parseNumberExpression(list, parser, input); break;
        case TokenTypeLParen: left = parseGroupedExpression(list, parser, input); break;
        case TokenTypeMinus: left = parseUnaryExpression(list, parser, input); break;
    }
    if (token > TokenTypeFunction)
        left = parseUnaryExpression(list, parser, input);
    if (parserCheckNull(parser, left))
        return NULL;
    while (parser->curr->type != TokenTypeEndOfInput && precedence < getPrecedence(parser->curr->type)) {
        switch (parser->curr->type) {
            case TokenTypePlus: case TokenTypeMinus: case TokenTypeTimes:
            case TokenTypeSlash: case TokenTypeExponent:
                left = parseBinaryExpression(list, parser, input, left); break;
            default:
                left = parseNoTimes(list, parser, input, left); break;
        }
    if (parserCheckNull(parser, left))
        return NULL;
    }
    return left;
}




