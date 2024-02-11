


#pragma once
#include "../tree/tree.h"


void* differentiateAbs(List* list, UnaryExpression* expression);
void* differentiateSqrt(List* list, UnaryExpression* expression);
void* differentiateLn(List* list, UnaryExpression* expression);
void* differentiateLog(List* list, UnaryExpression* expression);

void* differentiateSin(List* list, UnaryExpression* expression);
void* differentiateCos(List* list, UnaryExpression* expression);
void* differentiateTan(List* list, UnaryExpression* expression);
void* differentiateCot(List* list, UnaryExpression* expression);
void* differentiateSec(List* list, UnaryExpression* expression);
void* differentiateCsc(List* list, UnaryExpression* expression);

void* differentiateArcsin(List* list, UnaryExpression* expression);
void* differentiateArccos(List* list, UnaryExpression* expression);
void* differentiateArctan(List* list, UnaryExpression* expression);
void* differentiateArccot(List* list, UnaryExpression* expression);
void* differentiateArcsec(List* list, UnaryExpression* expression);
void* differentiateArccsc(List* list, UnaryExpression* expression);

void* differentiateSinh(List* list, UnaryExpression* expression);
void* differentiateCosh(List* list, UnaryExpression* expression);
void* differentiateTanh(List* list, UnaryExpression* expression);
void* differentiateCoth(List* list, UnaryExpression* expression);
void* differentiateSech(List* list, UnaryExpression* expression);
void* differentiateCsch(List* list, UnaryExpression* expression);





