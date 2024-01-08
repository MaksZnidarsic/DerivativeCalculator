


#pragma once
#include "token.h"


enum ExpressionType {
    ExpressionTypeBinary, ExpressionTypeUnary,
    ExpressionTypeDependent, ExpressionTypeConstant,
    ExpressionTypeNumber,
};


enum OperatorPrecedence {
    OperatorPrecedenceLowest,
    OperatorPrecedenceSum,
    OperatorPrecedenceProduct,
    OperatorPrecedenceExponent,
    OperatorPrecedenceHighest,
};


typedef struct BinaryExpression BinaryExpression;
typedef struct UnaryExpression UnaryExpression;
typedef struct VariableExpression VariableExpression;
typedef struct NumberExpression NumberExpression;
typedef struct Parser Parser;
typedef struct List List;


struct BinaryExpression {
    enum ExpressionType type;
    enum TokenType op;
    void* left;
    void* right;
};


struct UnaryExpression {
    enum ExpressionType type;
    enum TokenType op;
    void* left;
};


struct VariableExpression {
    enum ExpressionType type;
    char ch;
};


struct NumberExpression {
    enum ExpressionType type;
    int num;
};


struct Parser {
    Token* curr;
    Token* next;
    unsigned int error;
};


struct List {
    void** data;
    unsigned int len;
};


BinaryExpression* createBinaryExpression(List* list, enum TokenType op, void* left, void* right);
UnaryExpression* createUnaryExpression(List* list, enum TokenType op, void* left);
VariableExpression* createConstantExpression(List* list, char ch);
VariableExpression* createDependentExpression(List* list, char ch);
NumberExpression* createNumberExpression(List* list, double num);
enum OperatorPrecedence getPrecedence(enum TokenType token);
void* parseExpression(List* list, Parser* parser, Input* input, enum OperatorPrecedence precedence);

Parser* createParser(Input* input);
void parserNext(Parser* parser, Input* input);
unsigned int parserCheckNull(Parser* parser, void* expression);

List* createList();
void listPushExpression(List* list, void* expression);

Parser* cleanParser(Parser* parser);
List* cleanList(List* list);





