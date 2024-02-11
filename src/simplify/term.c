


#include "term.h"
#include "simplify.h"
//order the file and so on


unsigned int checkEqualExpression(void* left, void* right) {
    if (*(int*)left != *(int*)right) return 0;
    switch (*(int*)left) {
        case ExpressionTypeBinary:
            if (((BinaryExpression*)left)->op != ((BinaryExpression*)right)->op) return 0;
            if (checkEqualExpression(((BinaryExpression*)left)->left, ((BinaryExpression*)right)->left) &&
                    checkEqualExpression(((BinaryExpression*)left)->right, ((BinaryExpression*)right)->right)) return 1;
            break;
        case ExpressionTypeUnary:
            if (((UnaryExpression*)left)->op != ((UnaryExpression*)right)->op) return 0;
            if (checkEqualExpression(((UnaryExpression*)left)->left, ((UnaryExpression*)right)->left)) return 1;
            break;
        case ExpressionTypeDependent: return 1;
        case ExpressionTypeConstant:
            if (((VariableExpression*)left)->ch == ((VariableExpression*)right)->ch) return 1;
            break;
        case ExpressionTypeNumber:
            if (((NumberExpression*)left)->num == ((NumberExpression*)right)->num) return 1;
            break;
    }
    return 0;
}


Term* createTerm() {
    Term* term = malloc(sizeof(Term));
    term->top = calloc(1, sizeof(TreeList));
    term->bot = calloc(1, sizeof(TreeList));
    return term;
}


void treeListPushExpression(TreeList* tree, void* expression) {
    if (!tree || !expression) return;
    if (!tree->data) {
        tree->data = malloc(sizeof(void*));
        tree->data[0] = expression;
        tree->len = 1; return;
    }
    tree->data = realloc(tree->data, sizeof(void*) * (tree->len + 1));
    tree->data[tree->len] = expression;
    tree->len++;
}


unsigned int checkAssembleOperator(enum TokenType optype, enum TokenType op) {
    switch ((int)optype) {
        case TokenTypeTimes:
            if (op == TokenTypeTimes || op == TokenTypeSlash) return 1;
            break;
        case TokenTypePlus:
            if (op == TokenTypePlus || op == TokenTypeMinus) return 1;
            break;
    }
    return 0;
}


void assembleBinaryBranch(List* list, TreeList* primary, TreeList* secundary, void* expression, enum TokenType op) {
        if (*(int*)expression == ExpressionTypeBinary && checkAssembleOperator(op, ((BinaryExpression*)expression)->op)) {
            assembleTerm(list, primary, secundary, expression, op); return;
        }
        void* left = simplify(list, expression);
        if (*(int*)left == ExpressionTypeBinary && checkAssembleOperator(op, ((BinaryExpression*)left)->op)) {
            assembleTerm(list, primary, secundary, left, op); return;
        }
        if (*(int*)left == ExpressionTypeUnary && ((UnaryExpression*)left)->op == TokenTypeMinus) {
            if (op == TokenTypeTimes) {
                treeListPushExpression(primary, ((UnaryExpression*)left)->left);
                primary->sign++; return;
            }
            treeListPushExpression(secundary, ((UnaryExpression*)left)->left); return;
        }
        treeListPushExpression(primary, left);
}


void assembleTerm(List* list, TreeList* primary, TreeList* secundary, BinaryExpression* expression, enum TokenType op) {
    enum TokenType token;
    if (op == TokenTypeTimes)
        token = TokenTypeSlash;
    else token = TokenTypeMinus;
    if (expression->op == token) {
        assembleBinaryBranch(list, primary, secundary, expression->left, op);
        assembleBinaryBranch(list, secundary, primary, expression->right, op);
        return;
    }
    assembleBinaryBranch(list, primary, secundary, expression->left, op);
    assembleBinaryBranch(list, primary, secundary, expression->right, op);
    return;
}


void* treeListBuildBinaryTree(List* list, TreeList* tree, enum TokenType op, unsigned int i) {
    if (i + 1 == tree->len)
        return tree->data[i];
    return createBinaryExpression(list, op, tree->data[i], treeListBuildBinaryTree(list, tree, op, i + 1));
}


void orderTreeList(TreeList* tree) {
    if (!tree || tree->len == 0) return;
    //to be made
}


void* termBuildBinaryTree(List* list, Term* term, enum TokenType op) {
    orderTreeList(term->top);
    orderTreeList(term->bot);
    void* top = treeListBuildBinaryTree(list, term->top, op, 0);
    if ((term->top->sign + term->bot->sign) % 2 == 1)
        top = createUnaryExpression(list, TokenTypeMinus, top);
    if (!term->bot->len)
        return top;

    void* bot = treeListBuildBinaryTree(list, term->bot, op, 0);
    if (op == TokenTypeTimes && *(int*)bot == ExpressionTypeNumber && ((NumberExpression*)bot)->num == 1)
        return top;
    if (op == TokenTypePlus && *(int*)bot == ExpressionTypeNumber && ((NumberExpression*)bot)->num == 0)
        return top;
    enum TokenType token;
    if (op == TokenTypeTimes)
        token = TokenTypeSlash;
    else token = TokenTypeMinus;
    if (!term->top->len && op == TokenTypePlus)
        return createUnaryExpression(list, TokenTypeMinus, bot);
    if (!term->top->len && op == TokenTypeTimes)
        return createNumberExpression(list, 0);
    return createBinaryExpression(list, token, top, bot);
}


void treeListRemoveElement(TreeList* tree, unsigned int element) {
    for (unsigned int i = element; i < tree->len - 1; i++)
        tree->data[i] = tree->data[i + 1];
    tree->len--;
    tree->data = realloc(tree->data, tree->len * sizeof(void*));
}


Term* cleanTerm(Term* term) {
    if (!term)
        return NULL;
    if (term->top)
        free(term->top);
    if (term->bot)
        free(term->bot);
    free(term);
    return NULL;
}









