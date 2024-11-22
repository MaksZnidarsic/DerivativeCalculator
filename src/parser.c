


#include "parser.h"
#include "tree.h"
#include <stdio.h>
#include <string.h>


void advance(Parser* parser) {
    parser->curr = parser->next;
    parser->next = scan(&parser->pos);
}

void initParser(Parser* parser, const char* start) {
    parser->pos = start;
    parser->len = strlen(start);
    advance(parser);
    advance(parser);
}

void* parseBot(Parser* parser) {
    return NULL;
}

void* parseMid(Parser* parser) {
    return NULL;
}

unsigned int checkTopOp(TokenType token) {
    return token == TokenPlus || token == TokenMinus;
}

void* parseTop(Parser* parser) {
    void* left = parseMid(parser);
    if (!checkTopOp(parser->curr.type)) return left;
    return left;
}

void* parse(const char* expression) {
    if (!expression) return NULL;
    Parser parser;
    initParser(&parser, expression);
    return parseTop(&parser);
}
