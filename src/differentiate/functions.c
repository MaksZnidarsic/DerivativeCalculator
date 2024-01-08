


#include "functions.h"
#include "differentiate.h"


void* differentiateAbs(List* list, UnaryExpression* expression) {
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, expression, expression->left);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateSqrt(List* list, UnaryExpression* expression) {
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), createNumberExpression(list, 2));
    return differentiate(list, createBinaryExpression(list, TokenTypeExponent, expression->left, right));
}


void* differentiateLn(List* list, UnaryExpression* expression) {
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), expression->left);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateSin(List* list, UnaryExpression* expression) {
    UnaryExpression* right = createUnaryExpression(list, TokenTypeCos, expression->left);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateCos(List* list, UnaryExpression* expression) {
    UnaryExpression* right = createUnaryExpression(list, TokenTypeSin, expression->left);
    BinaryExpression* in = createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
    return createUnaryExpression(list, TokenTypeMinus, in);
}


void* differentiateTan(List* list, UnaryExpression* expression) {
    UnaryExpression* left = createUnaryExpression(list, TokenTypeSec, expression->left);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeExponent, left, createNumberExpression(list, 2));
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateCot(List* list, UnaryExpression* expression) {
    UnaryExpression* left = createUnaryExpression(list, TokenTypeCsc, expression->left);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeExponent, left, createNumberExpression(list, 2));
    BinaryExpression* in = createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
    return createUnaryExpression(list, TokenTypeMinus, in);
}


void* differentiateSec(List* list, UnaryExpression* expression) {
    UnaryExpression* right = createUnaryExpression(list, TokenTypeSec, expression->left);
    UnaryExpression* left = createUnaryExpression(list, TokenTypeTan, expression->left);
    BinaryExpression* in = createBinaryExpression(list, TokenTypeTimes, left, right);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), in);
}


void* differentiateCsc(List* list, UnaryExpression* expression) {
    UnaryExpression* right = createUnaryExpression(list, TokenTypeCsc, expression->left);
    UnaryExpression* left = createUnaryExpression(list, TokenTypeCot, expression->left);
    BinaryExpression* in = createBinaryExpression(list, TokenTypeTimes, left, right);
    BinaryExpression* inin = createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), in);
    return createUnaryExpression(list, TokenTypeMinus, inin);
}


void* differentiateArcsin(List* list, UnaryExpression* expression) {
    BinaryExpression* botright = createBinaryExpression(list, TokenTypeExponent, expression->left, createNumberExpression(list, 2));
    BinaryExpression* bot = createBinaryExpression(list, TokenTypeMinus, createNumberExpression(list, 1), botright);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), createUnaryExpression(list, TokenTypeSqrt, bot));
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateArccos(List* list, UnaryExpression* expression) {
    return createUnaryExpression(list, TokenTypeMinus, differentiateArcsin(list, expression));
}


void* differentiateArctan(List* list, UnaryExpression* expression) {
    BinaryExpression* botright = createBinaryExpression(list, TokenTypeExponent, expression->left, createNumberExpression(list, 2));
    BinaryExpression* bot = createBinaryExpression(list, TokenTypePlus, createNumberExpression(list, 1), botright);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), bot);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateArccot(List* list, UnaryExpression* expression) {
    void* out = createUnaryExpression(list, TokenTypeMinus, differentiateArctan(list, expression));
    return out;
}


void* differentiateArcsec(List* list, UnaryExpression* expression) {
    BinaryExpression* botleft = createBinaryExpression(list, TokenTypeExponent, expression->left, createNumberExpression(list, 2));
    BinaryExpression* botinright = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), botleft);
    BinaryExpression* botin = createBinaryExpression(list, TokenTypeMinus, createNumberExpression(list, 1), botinright);
    UnaryExpression* botright = createUnaryExpression(list, TokenTypeSqrt, botin);
    BinaryExpression* bot = createBinaryExpression(list, TokenTypeTimes, botleft, botright);
    BinaryExpression* right = createBinaryExpression(list, TokenTypeSlash, createNumberExpression(list, 1), bot);
    return createBinaryExpression(list, TokenTypeTimes, differentiate(list, expression->left), right);
}


void* differentiateArccsc(List* list, UnaryExpression* expression) {
    return createUnaryExpression(list, TokenTypeMinus, differentiateArcsec(list, expression));
}





