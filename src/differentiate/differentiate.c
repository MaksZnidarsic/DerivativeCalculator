


#include "differentiate.h"
#include "functions.h"


BinaryExpression* sumRule(List* list, BinaryExpression* expression) {
    return createBinaryExpression(list, expression->op, differentiate(list, expression->left), differentiate(list, expression->right));
}


BinaryExpression* productRule(List* list, BinaryExpression* expression) {
    BinaryExpression* left = createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), expression->right);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeTimes, expression->left, differentiate(list, expression->right));
    return createBinaryExpression(list, TokenTypePlus, left, right);
}


BinaryExpression* quotientRule(List* list, BinaryExpression* expression) {
    BinaryExpression* left = createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), expression->right);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeTimes, expression->left, differentiate(list, expression->right));
    BinaryExpression* top = createBinaryExpression(list, TokenTypeMinus, left, right);
    BinaryExpression* bot = createBinaryExpression(list, TokenTypeExponent, expression->right, createNumberExpression(list, 2));
    return createBinaryExpression(list, TokenTypeSlash, top, bot);
}


BinaryExpression* differentiateExponent(List* list, BinaryExpression* expression) {
    UnaryExpression* ln = createUnaryExpression(list, TokenTypeLn, expression->left);
    BinaryExpression* left  = createBinaryExpression(list, TokenTypeTimes, ln, expression->right);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, left), expression);
}


BinaryExpression* differentiateBinaryExpression(List* list, BinaryExpression* expression) {
    if (!expression)
        return NULL;
    switch ((int)expression->op) {
        case TokenTypePlus:
        case TokenTypeMinus:
            return sumRule(list, expression);
        case TokenTypeTimes:
            return productRule(list, expression);
        case TokenTypeSlash:
            return quotientRule(list, expression);
        case TokenTypeExponent:
            return differentiateExponent(list, expression);
    }
    return NULL;
}


void* differentiateUnaryMinus(List* list, UnaryExpression* expression) {
    return createUnaryExpression(list, TokenTypeMinus, differentiate(list, expression->left));
}


UnaryExpression* differentiateUnaryExpression(List* list, UnaryExpression* expression) {
    if (!expression)
        return NULL;
    switch((int)expression->op) {
        case TokenTypeMinus: return differentiateUnaryMinus(list, expression);
        case TokenTypeAbs: return differentiateAbs(list, expression);
        case TokenTypeSqrt: return differentiateSqrt(list, expression);
        case TokenTypeLn: return differentiateLn(list, expression);
        case TokenTypeSin: return differentiateSin(list, expression);
        case TokenTypeCos: return differentiateCos(list, expression);
        case TokenTypeTan: return differentiateTan(list, expression);
        case TokenTypeCot: return differentiateCot(list, expression);
        case TokenTypeSec: return differentiateSec(list, expression);
        case TokenTypeCsc: return differentiateCsc(list, expression);
        case TokenTypeArcsin: return differentiateArcsin(list, expression);
        case TokenTypeArccos: return differentiateArccos(list, expression);
        case TokenTypeArctan: return differentiateArctan(list, expression);
        case TokenTypeArccot: return differentiateArccot(list, expression);
        case TokenTypeArcsec: return differentiateArcsec(list, expression);
        case TokenTypeArccsc: return differentiateArccsc(list, expression);
    }
    return NULL;
}


NumberExpression* differentiateDependentExpression(List* list) {
    return createNumberExpression(list, 1);
}


NumberExpression* differentiateConstantExpression(List* list) {
    return createNumberExpression(list, 0);
}


void* differentiate(List* list, void* expression) {
    if (!expression)
        return NULL;
    switch (*(int*)expression) {
        case ExpressionTypeBinary:
            return differentiateBinaryExpression(list, expression);
        case ExpressionTypeUnary:
            return differentiateUnaryExpression(list, expression);
        case ExpressionTypeDependent:
            return differentiateDependentExpression(list);
        case ExpressionTypeConstant:
        case ExpressionTypeNumber:
            return differentiateConstantExpression(list);
    }
    return NULL;
}





