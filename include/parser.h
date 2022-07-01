/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

typedef enum {
    WCC_NODE_FLOAT,
    WCC_NODE_INT,
    WCC_NODE_STRING,
    WCC_NODE_CHAR,

    WCC_NODE_BINOP,
    WCC_NODE_UNOP,

    WCC_NODE_VARACCESS,
    WCC_NODE_FUNCCALL, /* Both a statement and expression */

    WCC_NODE_FOR,
    WCC_NODE_WHILE,
    WCC_NODE_IF,
    WCC_NODE_RETURN,
    WCC_NODE_CONTINUE,
    WCC_NODE_BREAK,

    WCC_NODE_STRUCT,
    WCC_NODE_ENUM,
    WCC_NODE_TYPEDEF,

    WCC_NODE_VARDECL,
    WCC_NODE_FUNCDECL,

    WCC_NODE_COMPOUND,
} wccNodeType;

typedef struct {
    wccNodeType type;
    void* node;
} wccASTNode;

typedef struct {
    wccToken* value;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccConstNode; /* FLOAT, INT, CHAR, STRING, VARACCESS */

typedef struct {
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccControlFlowNode; /* BREAK, CONTINUE (we don't need to keep track of token here) */

typedef struct {
    wccToken* op;
    wccASTNode* left;
    wccASTNode* right;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccBinOpNode; /* ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, SHL, SHR, BIT_AND, BIT_OR, BIT_XOR, BIT_NOT */

typedef struct {
    wccToken* op;
    wccASTNode* expr;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccUnOpNode; /* NOT, INC, DEC, DEREF */

typedef struct {
    wccASTNode* initializer;
    wccASTNode* condition;
    wccASTNode* increment;
    wccASTNode* body;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccForNode;

typedef struct {
    wccASTNode* condition;
    wccASTNode* body;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccWhileNode;

typedef struct {
    size_t cases;
    wccASTNode** condition;
    wccASTNode** body;
    wccASTNode* elseBody;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccIfNode;

typedef struct {
    wccASTNode* expr;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccReturnNode;

typedef struct {
    wccASTNode* ret_type;
    wccToken* name;
    wccASTNode* parameters;
    wccASTNode* body;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccFuncDeclNode;

typedef struct {
    wccASTNode* type;
    wccToken* name;
    wccASTNode* initializer;
    bool hasInitializer;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccVarDeclNode;

typedef struct {
    wccToken* name;
    wccASTNode** args;
    size_t args_count;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccFuncCallNode;

typedef struct {
    wccASTNode* type;
    wccToken* name;
    wccASTNode* body;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccStructNode;

typedef struct {
    wccToken* name;
    wccASTNode** args;
    size_t args_count;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccEnumNode;

typedef struct {
    wccToken* name;
    wccASTNode* body;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccTypedefNode;

typedef struct {
    wccASTNode** statements;
    size_t statements_count;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccCompoundNode;

/**********************************************/

typedef struct {
    wccToken* tok;
    wccTokenList* tokens;
    size_t idx;
    char* src;
    char* filename;
} wccParserCtx;

wccASTNode* wccParse(char* src, char* filename, wccTokenList* tokens);

wccConstNode* wccParseConst(wccParserCtx* ctx);
wccControlFlowNode* wccParseControlFlow(wccParserCtx* ctx);
wccBinOpNode* wccParseBinOp(wccParserCtx* ctx);
wccUnOpNode* wccParseUnOp(wccParserCtx* ctx);
wccForNode* wccParseFor(wccParserCtx* ctx);
wccWhileNode* wccParseWhile(wccParserCtx* ctx);
wccIfNode* wccParseIf(wccParserCtx* ctx);
wccReturnNode* wccParseReturn(wccParserCtx* ctx);
wccFuncDeclNode* wccParseFuncDecl(wccParserCtx* ctx);
wccVarDeclNode* wccParseVarDecl(wccParserCtx* ctx);
wccFuncCallNode* wccParseFuncCall(wccParserCtx* ctx);
wccStructNode* wccParseStruct(wccParserCtx* ctx);
wccEnumNode* wccParseEnum(wccParserCtx* ctx);
wccTypedefNode* wccParseTypedef(wccParserCtx* ctx);
wccCompoundNode* wccParseCompound(wccParserCtx* ctx);

wccASTNode* wccParseStatement(wccParserCtx* ctx);
wccASTNode* wccParseExpression(wccParserCtx* ctx);

wccASTNode* wccParseProgram(wccParserCtx* ctx);

