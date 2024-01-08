


#pragma once
#include "../tree/tree.h"


typedef struct Term Term;
typedef struct TreeList TreeList;

struct Term {
    TreeList* top;
    TreeList* bot;
};


struct TreeList {
    void** data;
    unsigned int len, sign;
};


unsigned int checkEqualExpression(void* left, void* right);

Term* createTerm();
void treeListPushExpression(TreeList* tree, void* expression);
void assembleTerm(List* list, TreeList* primary, TreeList* secundary, BinaryExpression* expression, enum TokenType op);
void* termBuildBinaryTree(List* list, Term* term, enum TokenType op);
void treeListRemoveElement(TreeList* tree, unsigned int element);
Term* cleanTerm(Term* term);





