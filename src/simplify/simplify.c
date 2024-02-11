

#include "simplify.h"
#include "term.h"
//to do: rename functions, general optimization


void separateArgument(List* list, void* expression, void** left, void** argleft, enum TokenType token) {
    if (*(int*)expression == ExpressionTypeBinary && ((BinaryExpression*)expression)->op == token) {
        *left = ((BinaryExpression*)expression)->left;
        *argleft = ((BinaryExpression*)expression)->right;
    }
    else {
        *left = expression;
        *argleft = createNumberExpression(list, 1);
    }
}


void removePaired(TreeList* tree, unsigned int i, unsigned int j) {
    if (i == j)
        return;
    if (i > j) {
        treeListRemoveElement(tree, i);
        treeListRemoveElement(tree, j); return;
    }
    treeListRemoveElement(tree, j);
    treeListRemoveElement(tree, i);
}


void removeCanceled(List* list, Term* term, unsigned int i, unsigned int j) {
    treeListRemoveElement(term->top, i);
    treeListRemoveElement(term->bot, j);
    if (term->top->len == 0)
        treeListPushExpression(term->top, createNumberExpression(list, 1));
}


unsigned int simplifyPair(List* list, TreeList* tree, void* left, void* right, void* argleft, void* argright, enum TokenType token) {
    if (checkEqualExpression(left, right)) {
        BinaryExpression* top = createBinaryExpression(list, TokenTypePlus, argleft, argright);
        treeListPushExpression(tree, simplify(list, createBinaryExpression(list, token, left, top)));
        return 1;
    }
    return 0;
}


//conditions to add to bot
unsigned int simplifyCancel(List* list, Term* term, void* left, void* right, void* argleft, void* argright, enum TokenType token) {
    if (checkEqualExpression(left, right)) {
        BinaryExpression* top = createBinaryExpression(list, TokenTypeMinus, argleft, argright);
        treeListPushExpression(term->top, simplify(list, createBinaryExpression(list, token, left, top)));
        return 1;
    }
    return 0;
}


unsigned int checkPairUp(List* list, TreeList* tree, unsigned int i, unsigned int j, enum TokenType token) {
    void* left, *right, *argleft, *argright;
    separateArgument(list, tree->data[i], &left, &argleft, token);
    separateArgument(list, tree->data[j], &right, &argright, token);
    if (simplifyPair(list, tree, left, right, argleft, argright, token)) {
        removePaired(tree, i, j); return 1;
    }
    if (token == TokenTypeTimes) {
        if (!(*(int*)argleft == ExpressionTypeNumber && ((NumberExpression*)argleft)->num == 1))
            if (simplifyPair(list, tree, argleft, argright, left, right, token)) {
                removePaired(tree, i, j); return 1;
            }
        if (!(*(int*)left == ExpressionTypeNumber && ((NumberExpression*)left)->num == 1))
            if (simplifyPair(list, tree, left, argright, argleft, right, token)) {
                removePaired(tree, i, j); return 1;
            }
        if (!(*(int*)argleft == ExpressionTypeNumber && ((NumberExpression*)argleft)->num == 1)) {
            if (simplifyPair(list, tree, argleft, right, left, argright, token)) {
                removePaired(tree, i, j); return 1;
            }
        }
    }
    return 0;
}


unsigned int checkCancelOut(List* list, Term* term, unsigned int i, unsigned int j, enum TokenType token) {
    void* left, *right, *argleft, *argright;
    separateArgument(list, term->top->data[i], &left, &argleft, token);
    separateArgument(list, term->bot->data[j], &right, &argright, token);
    if (simplifyCancel(list, term, left, right, argleft, argright, token)) {
        removeCanceled(list, term, i, j); return 1;
    }
    if (token == TokenTypeTimes) {
        if (!(*(int*)argleft == ExpressionTypeNumber && ((NumberExpression*)argleft)->num == 1))
            if (simplifyCancel(list, term, argleft, argright, left, right, token)) {
                removeCanceled(list, term, i, j); return 1;
            }
        if (!(*(int*)left == ExpressionTypeNumber && ((NumberExpression*)left)->num == 1))
            if (simplifyCancel(list, term, left, argright, argleft, right, token)) {
                removeCanceled(list, term, i, j); return 1;
            }
        if (!(*(int*)argleft == ExpressionTypeNumber && ((NumberExpression*)argleft)->num == 1))
            if (simplifyCancel(list, term, argleft, right, left, argright, token)) {
                removeCanceled(list, term, i, j); return 1;
            }
    }
    return 0;
}


void pairUpSum(List* list, TreeList* tree) {
    for (int i = 0; i < tree->len; i++)
        if (*(int*)tree->data[i] == ExpressionTypeNumber && ((NumberExpression*)tree->data[i])->num == 0 && tree->len != 1) {
            treeListRemoveElement(tree, i); i--;
        }
    for (int i = 0; i < tree->len; i++)
        for (int j = 0; j < tree->len; j++) {
            if (i == j) continue;
            if (*(int*)tree->data[i] == ExpressionTypeNumber && *(int*)tree->data[j] == ExpressionTypeNumber) {
                treeListPushExpression(tree, createNumberExpression(list, ((NumberExpression*)tree->data[i])->num + ((NumberExpression*)tree->data[j])->num));
                removePaired(tree, i, j);
                i--; break;
            }
            if (checkPairUp(list, tree, i, j, TokenTypeTimes)) {
                i--; break;
            }
        }
}


void cancelOutSum(List* list, Term* term) {
    for (int i = 0; i < term->top->len; i++)
        for (unsigned int j = 0; j < term->bot->len; j++) {
            if (*(int*)term->top->data[i] == ExpressionTypeNumber && *(int*)term->bot->data[j] == ExpressionTypeNumber) {
                treeListPushExpression(term->top, createNumberExpression(list, ((NumberExpression*)term->top->data[i])->num - ((NumberExpression*)term->bot->data[j])->num));
                removeCanceled(list, term, i, j);
                i--; break;
            }
            if (checkCancelOut(list, term, i, j, TokenTypeTimes)) {
                i--; break;
            }
        }
}


void* simplifySum(List* list, BinaryExpression* expression) {
    Term* term = createTerm();
    assembleTerm(list, term->top, term->bot, expression, TokenTypePlus);
    pairUpSum(list, term->top);
    if (term->bot->len) {
        pairUpSum(list, term->bot);
        cancelOutSum(list, term);
    }
    void* r = termBuildBinaryTree(list, term, TokenTypePlus);
    term = cleanTerm(term);
    return r;
}


unsigned int pairUpProduct(List* list, TreeList* tree) {
    for (int i = 0; i < tree->len; i++) {
        if (*(int*)tree->data[i] == ExpressionTypeNumber && ((NumberExpression*)tree->data[i])->num == 0) {
            return 1;
        }
        if (*(int*)tree->data[i] == ExpressionTypeNumber && ((NumberExpression*)tree->data[i])->num == 1 && tree->len != 1) {
            treeListRemoveElement(tree, i); i--;
        }
    }
    for (int i = 0; i < tree->len; i++)
        for (unsigned int j = 0; j < tree->len; j++) {
            if (i == j) continue;
            if (*(int*)tree->data[i] == ExpressionTypeNumber && *(int*)tree->data[j] == ExpressionTypeNumber) {
                treeListPushExpression(tree, createNumberExpression(list, ((NumberExpression*)tree->data[i])->num * ((NumberExpression*)tree->data[j])->num));
                removePaired(tree, i, j);
                i--; break;
            }
            if (checkPairUp(list, tree, i, j, TokenTypeExponent)) {
                i--; break;
            }
        }
    return 0;
}


void cancelOutProduct(List* list, Term* term) {
    for (int i = 0; i < term->top->len; i++)
        for (int j = 0; j < term->bot->len; j++) {
            if (checkCancelOut(list, term, i, j, TokenTypeExponent)) {
                i--; break;
            }
        }
}


void* simplifyProduct(List* list, void* expression) {
    Term* term = createTerm();
    assembleTerm(list, term->top, term->bot, expression, TokenTypeTimes);
    if (pairUpProduct(list, term->top)) {
        term = cleanTerm(term);
        return createNumberExpression(list, 0);
    }
    if (term->bot->len) {
        pairUpProduct(list, term->bot);
        cancelOutProduct(list, term);
    }
    void* r = termBuildBinaryTree(list, term, TokenTypeTimes);
    term = cleanTerm(term);
    return r;
}


//to be simplified
void* simplifyBinaryExponent(List* list, BinaryExpression* expression) {
    void* left = simplify(list, expression->left);
    void* right = simplify(list, expression->right);
    if (*(int*)left == ExpressionTypeNumber && *(int*)right == ExpressionTypeNumber)
        return simplify(list, createNumberExpression(list, pow(((NumberExpression*)left)->num, ((NumberExpression*)right)->num)));
    if (*(int*)left == ExpressionTypeBinary && ((BinaryExpression*)left)->op == TokenTypeExponent) {
        BinaryExpression* top = createBinaryExpression(list, TokenTypeTimes, ((BinaryExpression*)left)->right, right);
        return simplify(list, createBinaryExpression(list, TokenTypeExponent, ((BinaryExpression*)left)->left, top));
    }
    if (*(int*)left == ExpressionTypeNumber && ((NumberExpression*)left)->num == 0)
        return createNumberExpression(list, 0);
    if (*(int*)left == ExpressionTypeNumber && ((NumberExpression*)left)->num == 1)
        return createNumberExpression(list, 1);
    if (*(int*)right == ExpressionTypeNumber && ((NumberExpression*)right)->num == 0)
        return createNumberExpression(list, 1);
    if (*(int*)right == ExpressionTypeNumber && ((NumberExpression*)right)->num == 1)
        return left;
    if (*(int*)left == ExpressionTypeBinary && ((BinaryExpression*)left)->op == TokenTypeExponent) {
        BinaryExpression* top = createBinaryExpression(list, TokenTypeTimes, ((BinaryExpression*)left)->right, right);
        return simplify(list, createBinaryExpression(list, TokenTypeExponent, ((BinaryExpression*)left)->left, top));
    }
    if (*(int*)left == ExpressionTypeConstant && ((VariableExpression*)left)->ch == 'e'
            && (*(int*)right == ExpressionTypeUnary && ((UnaryExpression*)right)->op == TokenTypeLn))
        return ((UnaryExpression*)right)->left;
    return createBinaryExpression(list, TokenTypeExponent, left, right);
}


void* simplifyBinaryExpression(List* list, BinaryExpression* expression) {
    switch ((int)expression->op) {
        case TokenTypePlus:
        case TokenTypeMinus:
            return simplifySum(list, expression);
        case TokenTypeTimes:
        case TokenTypeSlash:
            return simplifyProduct(list, expression);
        case TokenTypeExponent: return simplifyBinaryExponent(list, expression);
    }
    return NULL;
}


unsigned int checkOddExpression(enum TokenType token) {
    switch ((int)token) {
        case TokenTypeSin: case TokenTypeTan: case TokenTypeCot: case TokenTypeCsc:
        case TokenTypeArcsin: case TokenTypeArctan:
        case TokenTypeSinh: case TokenTypeTanh: case TokenTypeCoth: case TokenTypeCsch:
            return 1;
    }
    return 0;
}


unsigned int checkEvenExpression(enum TokenType token) {
    switch ((int)token) {
        case TokenTypeAbs:
        case TokenTypeCos: case TokenTypeSec:
        case TokenTypeCosh: case TokenTypeSech:
            return 1;
    }
    return 0;
}


void* simplifyUnaryExpression(List* list, UnaryExpression* expression) {
    void* left = simplify(list, expression->left);
    switch ((int)expression->op) {
        case TokenTypeMinus:
            if (*(int*)left == ExpressionTypeNumber && ((NumberExpression*)left)->num == 0)
                return left;
            if (*(int*)left == ExpressionTypeUnary && ((UnaryExpression*)left)->op == TokenTypeMinus)
                return ((UnaryExpression*)left)->left;
            break;
        case TokenTypeLn:
            if (*(int*)left == ExpressionTypeConstant && ((VariableExpression*)left)->ch == 'e')
                return createNumberExpression(list, 1);
            if (*(int*)left == ExpressionTypeBinary && ((BinaryExpression*)left)->op == TokenTypeExponent) {
                void* right = createUnaryExpression(list, TokenTypeLn, ((BinaryExpression*)left)->left);
                return createBinaryExpression(list, TokenTypeTimes, ((BinaryExpression*)left)->right, right);
            }
            break;
    }
    if (*(int*)left == ExpressionTypeUnary && ((UnaryExpression*)left)->op == TokenTypeMinus) {
        if (checkOddExpression(expression->op)) {
            UnaryExpression* in = createUnaryExpression(list, expression->op, ((UnaryExpression*)left)->left);
            return createUnaryExpression(list, TokenTypeMinus, in);
        }
        if (checkEvenExpression(expression->op))
            return createUnaryExpression(list, expression->op, ((UnaryExpression*)left)->left);
    }
    return createUnaryExpression(list, expression->op, left);
}


void* simplify(List* list, void* expression) {
    if (!expression)
        return NULL;
    switch (*(int*)expression) {
        case ExpressionTypeBinary: return simplifyBinaryExpression(list, expression);
        case ExpressionTypeUnary: return simplifyUnaryExpression(list, expression);
        case ExpressionTypeDependent:
        case ExpressionTypeConstant:
        case ExpressionTypeNumber:
            return expression;
    }
    return NULL;
}






