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
    WCC_NODE_STRING, /* TODO: Have casting done during parsing or somethign idk */
    WCC_NODE_CHAR,

    WCC_NODE_BINOP,
    WCC_NODE_UNOP,
    WCC_NODE_CAST,

    WCC_NODE_VARACCESS,
    WCC_NODE_FUNCCALL, /* Both a statement and expression */
    WCC_NODE_MEMBERACCESS,
    WCC_NODE_ARRAYACCESS,
    WCC_NODE_PTRACCESS,

    WCC_NODE_FOR,
    WCC_NODE_WHILE,
    WCC_NODE_IF,
    WCC_NODE_TERENARY,
    WCC_NODE_RETURN,
    WCC_NODE_CONTINUE,
    WCC_NODE_BREAK,

    WCC_NODE_STRUCT,
    WCC_NODE_ENUM,
    WCC_NODE_TYPEDEF,

    WCC_NODE_VARDECL,
    WCC_NODE_FUNCDECL,

    WCC_NODE_COMPOUND,
    WCC_NODE_LIST, // eg. 1, 2, 3
} wccNodeType;

typedef enum {
    WCC_TYPE_VOID,
    WCC_TYPE_CHAR,
    WCC_TYPE_INT,
    WCC_TYPE_FLOAT,
    WCC_TYPE_STRING,
    WCC_TYPE_STRUCT,
    WCC_TYPE_ENUM,
    WCC_TYPE_TYPEDEF,
    WCC_TYPE_AUTO,
    WCC_TYPE_UNKNOWN, // For typedef'd types, this'll be resolved in the compiler
} wccTypePrimary;

typedef enum {
    WCC_TYPE_IS_INLINE =   1 << 0,
    WCC_TYPE_IS_STATIC =   1 << 1,
    WCC_TYPE_IS_EXTERN =   1 << 2,
    WCC_TYPE_IS_CONST =    1 << 3,
    WCC_TYPE_IS_VOLATILE = 1 << 4,
    WCC_TYPE_IS_REGISTER = 1 << 5,
    WCC_TYPE_IS_RESTRICT = 1 << 6,
} wccTypeQualifier;

typedef struct {
    wccNodeType type;
    void* node;
} wccASTNode;

typedef struct {
    wccTypePrimary type;
    unsigned char ptr_depth;
    char attrib; // inline, static, const, volatile, etc.
} wccType;

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
    wccASTNode* lhs;
    wccASTNode* rhs;
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
    wccType* type;
    wccASTNode* expr;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccCastNode; /* CAST */

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
    wccASTNode** nodes;
    size_t count;
    size_t idx;
    size_t line;
    size_t column;
    size_t length;
} wccListNode;

typedef struct {
    wccASTNode* func;
    wccListNode* args;
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

typedef struct {
    wccASTNode* condition;
    wccASTNode* exprTrue;
    wccASTNode* exprFalse;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccTernaryNode;

typedef struct {
    wccASTNode* left;
    wccASTNode* right;
    size_t line;
    size_t column;
    size_t length;
    size_t idx;
} wccMemberAccessNode;

/**********************************************/

typedef struct {
    wccToken* tok;
    wccTokenList* tokens;
    size_t idx;
    char* src;
    char* filename;
} wccParserCtx;

wccASTNode* wccParse(char* src, char* filename, wccTokenList* tokens);

wccASTNode* wccParseStatement(wccParserCtx* ctx);
wccASTNode* wccParseExpression(wccParserCtx* ctx);

/* TODO: Gove these shits a name lmao */
wccASTNode* wccParsePrecedence1(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence2(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence3(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence4(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence5(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence6(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence7(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence8(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence9(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence10(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence11(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence12(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence13(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence14(wccParserCtx* ctx);
wccASTNode* wccParsePrecedence15(wccParserCtx* ctx);
wccASTNode* wccParsePrimary(wccParserCtx* ctx);

wccASTNode* wccParseProgram(wccParserCtx* ctx);

void wccPrintAST(wccASTNode* node, int indent);
