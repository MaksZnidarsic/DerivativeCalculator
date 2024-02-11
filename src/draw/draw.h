


#pragma once
#include "../tree/tree.h"
//to be rewritten


void printTokenString(enum TokenType token);
void printToken(Token* token);
void printTree(void* expression, unsigned int indent);
void printExpression(void* expression);
void printList(void* list);
void printLatex(void* expression);
