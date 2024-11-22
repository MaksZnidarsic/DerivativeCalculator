


#pragma once
#include "scanner.h"
//#define PARSER_DEBUG


typedef struct Parser {
    const char* pos;
    unsigned int len;
    Token curr, next;
#ifdef PARSER_DEBUG
    unsigned int curr_depth;
#endif
} Parser;

void* parse(const char* expression);
