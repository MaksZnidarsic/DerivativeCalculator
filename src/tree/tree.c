


#include "tree.h"


Parser* createParser(Input* input) {
    if (!input)
        return NULL;
    Parser* parser = malloc(sizeof(Parser));
    parser->curr = inputNext(input);
    parser->next = inputNext(input);
    parser->error = 0;
    return parser;
}


    void parserNext(Parser* parser, Input* input) {
    cleanToken(parser->curr);
    parser->curr = parser->next;
    parser->next = inputNext(input);
}


unsigned int parserCheckNull(Parser* parser, void* expression) {
    if (expression)
        return 0;
    parser->error++;
    return 1;
}


List* createList() {
    return calloc(1, sizeof(List));
}


void listPushExpression(List* list, void* expression) {
    if (!list || !expression) return;
    if (!list->data) {
        list->data = malloc(sizeof(void*));
        list->data[0] = expression;
        list->len = 1; return;
    }
    list->data = realloc(list->data, sizeof(void*) * (list->len + 1));
    list->data[list->len] = expression;
    list->len++;
}


Parser* cleanParser(Parser* parser) {
    if (!parser)
        return NULL;
    parser->curr = cleanToken(parser->curr);
    parser->next = cleanToken(parser->curr);
    free(parser);
    return NULL;
}


List* cleanList(List* list) {
    if (!list)
        return NULL;
    for (unsigned int i = 0; i < list->len; i++)
        if (list->data[i])
            switch (*(int*)list->data[i]) {
                case ExpressionTypeBinary:
                case ExpressionTypeUnary:
                case ExpressionTypeDependent:
                case ExpressionTypeConstant:
                case ExpressionTypeNumber:
                    free(list->data[i]);
            }
    free(list);
    return NULL;
}








