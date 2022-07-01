/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include "token.h"
#include "parser.h"

void advance(wccParserCtx *ctx) {
    ctx->idx++;
    ctx->tok = ctx->tokens->tokens[ctx->idx];
}

wccASTNode* wccParse(char* src, char* filename, wccTokenList* tokens) {
    wccParserCtx ctx = {
        .src = src,
        .filename = filename,
        .idx = -1,
        .tokens = tokens,
    };
    advance(&ctx);
    wccASTNode* ast = wccParseProgram(&ctx);
    return ast;
}

/*
wccConstNode* wccParseConst(wccParserCtx* ctx) {return NULL;};
wccControlFlowNode* wccParseControlFlow(wccParserCtx* ctx) {return NULL;};
wccBinOpNode* wccParseBinOp(wccParserCtx* ctx) {return NULL;};
wccUnOpNode* wccParseUnOp(wccParserCtx* ctx) {return NULL;};
wccForNode* wccParseFor(wccParserCtx* ctx) {return NULL;};
wccWhileNode* wccParseWhile(wccParserCtx* ctx) {return NULL;};
wccIfNode* wccParseIf(wccParserCtx* ctx) {return NULL;};
wccReturnNode* wccParseReturn(wccParserCtx* ctx) {return NULL;};
wccFuncDeclNode* wccParseFuncDecl(wccParserCtx* ctx) {return NULL;};
wccVarDeclNode* wccParseVarDecl(wccParserCtx* ctx) {return NULL;};
wccFuncCallNode* wccParseFuncCall(wccParserCtx* ctx) {return NULL;};
wccStructNode* wccParseStruct(wccParserCtx* ctx) {return NULL;};
wccEnumNode* wccParseEnum(wccParserCtx* ctx) {return NULL;};
wccTypedefNode* wccParseTypedef(wccParserCtx* ctx) {return NULL;};
wccCompoundNode* wccParseCompound(wccParserCtx* ctx) {return NULL;};

wccASTNode* wccParseExpression(wccParserCtx* ctx) {return NULL;};

wccASTNode* wccParseStatement(wccParserCtx* ctx) {
    return NULL;
}
*/
wccASTNode* wccParseProgram(wccParserCtx* ctx) {
    wccCompoundNode* program = malloc(sizeof(wccCompoundNode));
    program -> statements = malloc(sizeof(wccASTNode*) * 2),
    program -> statements_count = 0;
    program -> line = ctx -> tok -> line;
    program -> idx = ctx -> tok -> idx;
    program -> column = ctx -> tok -> col;

    size_t cap = 2;
    bool error = false;

    wccASTNode* ast = malloc(sizeof(wccASTNode));
    while (ctx -> tok -> type != WCC_TOKEN_EOF) {
        if (program -> statements_count == cap) {
            cap *= 2;
            program -> statements = realloc(program -> statements, sizeof(wccASTNode*) * cap);
        }
        // program -> statements[program -> statements_count++] = wccParseStatement(ctx);
        if (program -> statements[program -> statements_count - 1] == NULL) {
            error = true;
        }
    }
    ast -> type = WCC_NODE_COMPOUND;
    ast -> node = program;
    if (error) {
        exit(1); // All errors have been printed, we can now exit
    }
    return ast;
}
