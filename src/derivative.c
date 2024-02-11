


#include "derivative.h"
#include "differentiate/differentiate.h"
#include "simplify/simplify.h"
#include "draw/draw.h"


void clean(Input* input, Parser* parser, List* list) {
    input = cleanInput(input);
    parser = cleanParser(parser);
    list = cleanList(list);
}


void diff(const char* input, char variable) {
    if (!input) return;
    Input* I = createInput(input, variable);
    Parser* P = createParser(I);
    List* L = createList();

    void* expression = simplify(L, parseExpression(L, P, I, 0));
    if (!expression) {
        printf("\nsyntax error\n       ^^^^^");
        clean(I, P, L);
    }

    void* derivative = simplify(L, differentiate(L, expression));
    printf("d/d%c ", variable); printExpression(expression);
    printf(" = "); printExpression(derivative);
}





