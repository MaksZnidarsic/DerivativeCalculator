


#pragma once


typedef enum NodeType {
    NodeBinary, NodeCliqueProd, NodeCliqueSum, NodeNum, NodeUnary, NodeVar,
} NodeType;

typedef enum OperatorType {
    OpPlus, OpTimes,
    OpDiv, OpMinus,
    OpCos, OpCot, OpCsc, OpLn, OpSec, OpSin, OpTan
} OperatorType;

typedef struct Binary {
    NodeType type;
    void* left, * right;
    OperatorType op;
} Binary;

typedef struct Clique {
    NodeType type;
    void* top, * bot;
} Clique;

typedef struct Num {
    NodeType type;
    unsigned int val;
} Num;

typedef struct Unary {
    NodeType type;
    void* right;
    OperatorType op;
} Unary;

typedef struct Var {
    NodeType type;
    const char* lit;
    unsigned int len;
} Var;

typedef struct Stash {
    unsigned int len, size, ptr_len;
    void* data, ** ptrs;
} Stash;


Binary* makeBinary(Stash* stash);
Clique* makeClique(Stash* stash);
Num* makeNum(Stash* stash);
Unary* makeUnary(Stash* stash);
Var* makeVar(Stash* stash);
