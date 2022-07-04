/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include <string.h>
#include "token.h"
#include "parser.h"

#define wccParseBinOpExpr(higherPrecedence, condition) \
    wccASTNode* lhs = wccParsePrecedence##higherPrecedence(ctx);\
    while (condition) {\
        wccToken* op = ctx->tok;\
        advance(ctx);\
        wccASTNode* rhs = wccParsePrecedence##higherPrecedence(ctx);\
        wccBinOpNode* node = malloc(sizeof(wccBinOpNode));\
        node->op = op;\
        node->lhs = malloc(sizeof(wccASTNode));\
        node->lhs->type = lhs->type;\
        node->lhs->node = lhs->node;\
        node->rhs = rhs;\
        lhs->type = WCC_NODE_BINOP;\
        lhs->node = node;\
    }\
    return lhs


void advance(wccParserCtx *ctx) {
    ctx->idx++;
    if (ctx->idx < ctx->tokens->size) {
        ctx->tok = ctx->tokens->tokens[ctx->idx];
    } else {
        ctx->tok = NULL;
    }
}

void save_pos(wccParserCtx *ctx) {
    ctx->saved_idx = ctx->idx;
}

void restore_pos(wccParserCtx *ctx) {
    ctx->idx = ctx->saved_idx;
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

wccType* wccParseType(wccParserCtx* ctx) {
    wccType* type = malloc(sizeof(wccType));
    type->type = WCC_TYPE_VOID;
    advance(ctx);
    return type;
}

wccASTNode* wccParsePrimary(wccParserCtx* ctx) {
    wccASTNode* node = malloc(sizeof(wccASTNode));
    switch (ctx->tok->type) {
        case WCC_TOKEN_IDENTIFIER: {
            wccConstNode* varAccess = malloc(sizeof(wccConstNode));
            varAccess->value = ctx->tok;
            varAccess->line = ctx->tok->line;
            varAccess->column = ctx->tok->col;
            varAccess->length = ctx->tok->len;
            varAccess->idx = ctx->tok->idx;
            advance(ctx);
            node->type = WCC_NODE_VARACCESS;
            node->node = varAccess;
        } break;
        case WCC_TOKEN_INT: {
            wccConstNode* constNode = malloc(sizeof(wccConstNode));
            constNode->value = ctx->tok;
            constNode->line = ctx->tok->line;
            constNode->column = ctx->tok->col;
            constNode->length = ctx->tok->len;
            constNode->idx = ctx->tok->idx;
            advance(ctx);
            node->type = WCC_NODE_INT;
            node->node = constNode;
        } break;
        case WCC_TOKEN_LPAREN: {
            advance(ctx);
            free(node); /* Fix redundancy */
            node = wccParseExpression(ctx);
            if (ctx->tok->type != WCC_TOKEN_RPAREN) {
                // TODO: Error
                return NULL;
            }
            advance(ctx);
        } break;
        default:
            // TODO: Error
            return NULL;
    }
    return node;
}

wccASTNode* wccParsePrecedence1(wccParserCtx* ctx) {
    wccASTNode* lhs = wccParsePrimary(ctx);
    if (ctx->tok->type == WCC_TOKEN_INC || ctx->tok->type == WCC_TOKEN_DEC) {
        wccUnOpNode* node = malloc(sizeof(wccUnOpNode));
        node->op = ctx->tok;
        node->expr = lhs;
        advance(ctx);
        wccASTNode* real_node = malloc(sizeof(wccASTNode));
        real_node->type = WCC_NODE_UNOP;
        real_node->node = node;
        lhs = real_node;
    } else if (ctx->tok->type == WCC_TOKEN_LPAREN) {
        advance(ctx);
        wccFuncCallNode* node = malloc(sizeof(wccFuncCallNode));
        node->args = malloc(sizeof(wccListNode));
        node->args->nodes = malloc(sizeof(wccASTNode*));
        node->args->count = 0;
        if (ctx->tok->type == WCC_TOKEN_RPAREN) {
            advance(ctx);
            node->func = malloc(sizeof(wccASTNode));
            node->func->type = lhs->type;
            node->func->node = lhs->node;
        } else {
            if (ctx->tok->type != WCC_TOKEN_RPAREN) {
                // TODO: Error for no closing paren
                return NULL;
            }
            advance(ctx);
        }
        lhs->type = WCC_NODE_FUNCCALL;
        lhs->node = node;
    } else if (ctx->tok->type == WCC_TOKEN_LBRACKET) {
        advance(ctx);
        wccASTNode* index = wccParseExpression(ctx);
        if (ctx->tok->type != WCC_TOKEN_RBRACKET) {
            // TODO: Error for no closing bracket
            return NULL;
        }
        advance(ctx);
        wccArrayAccessNode* node = malloc(sizeof(wccArrayAccessNode));
        node->arr = lhs;
        node->index = index;
        lhs->type = WCC_NODE_ARRAYACCESS;
        lhs->node = node;
    } else if (ctx->tok->type == WCC_TOKEN_DOT) {
        advance(ctx);
        wccMemberAccessNode* node = malloc(sizeof(wccMemberAccessNode));
        lhs->type = WCC_NODE_MEMBERACCESS;
        node->left = lhs;
        advance(ctx);
        node->right = wccParsePrecedence1(ctx); /* Here we can parse recursively as we want to allow for `foo.bar.baz` which expands to Access(foo, Access(bar, baz)) */
        lhs->node = node;
    } else if (ctx->tok->type == WCC_TOKEN_ARROW) {
        advance(ctx);
        wccMemberAccessNode* node = malloc(sizeof(wccMemberAccessNode));
        node->left = lhs;
        advance(ctx);
        node->right = wccParsePrecedence1(ctx); 
        lhs->type = WCC_NODE_MEMBERACCESS;
        lhs->node = node;
    }
    return lhs;
}

wccASTNode* wccParsePrecedence2(wccParserCtx* ctx) {
    // Could've written this better, but I cba
    wccASTNode* node = malloc(sizeof(wccASTNode));
    if (ctx->tok->type == WCC_TOKEN_INC || ctx->tok->type == WCC_TOKEN_DEC || ctx->tok->type == WCC_TOKEN_SUB|| ctx->tok->type == WCC_TOKEN_ADD || ctx->tok->type == WCC_TOKEN_NOT || ctx->tok->type == WCC_TOKEN_BIT_NOT || ctx->tok->type == WCC_TOKEN_MUL || ctx->tok->type == WCC_TOKEN_BIT_AND) {
        wccUnOpNode* unary = malloc(sizeof(wccUnOpNode));
        node->type = WCC_NODE_UNOP;
        unary->op = ctx->tok;
        advance(ctx);
        unary->expr = wccParsePrecedence1(ctx);
        node->node = unary;
    } else if (ctx->tok->type == WCC_TOKEN_LPAREN) {
        save_pos(ctx);
        // Cast
        advance(ctx);
        if (ctx->tok->type != WCC_TOKEN_KEYWORD) {
            restore_pos(ctx);
            free(node);
            return wccParsePrecedence1(ctx);
        }
        wccCastNode* cast = malloc(sizeof(wccCastNode));
        cast->type = wccParseType(ctx);
        if (ctx->tok->type != WCC_TOKEN_RPAREN) {
            // TODO: Throw error, expected ')'
            return NULL;
        }
        advance(ctx);
        cast->expr = wccParsePrecedence1(ctx);
        node->type = WCC_NODE_CAST;
        node->node = cast;
    } else {
        node = wccParsePrecedence1(ctx);
    }
    return node;
}

wccASTNode* wccParsePrecedence3(wccParserCtx* ctx) {
    wccParseBinOpExpr(2, ctx->tok->type == WCC_TOKEN_MUL || ctx->tok->type == WCC_TOKEN_DIV || ctx->tok->type == WCC_TOKEN_MOD);
}

wccASTNode* wccParsePrecedence4(wccParserCtx* ctx) {
    wccParseBinOpExpr(3, ctx->tok->type == WCC_TOKEN_ADD || ctx->tok->type == WCC_TOKEN_SUB);
}

wccASTNode* wccParsePrecedence5(wccParserCtx* ctx) {
    wccParseBinOpExpr(4, ctx->tok->type == WCC_TOKEN_SHR || ctx->tok->type == WCC_TOKEN_SHL);
}

wccASTNode* wccParsePrecedence6(wccParserCtx* ctx) {
    wccParseBinOpExpr(5, ctx->tok->type >= WCC_TOKEN_LT && ctx->tok->type <= WCC_TOKEN_GE);
}

wccASTNode* wccParsePrecedence7(wccParserCtx* ctx) {
    wccParseBinOpExpr(6, ctx->tok->type == WCC_TOKEN_EQ || ctx->tok->type == WCC_TOKEN_NE);
}

wccASTNode* wccParsePrecedence8(wccParserCtx* ctx) {
    wccParseBinOpExpr(7, ctx->tok->type == WCC_TOKEN_BIT_AND);
}

wccASTNode* wccParsePrecedence9(wccParserCtx* ctx) {
    wccParseBinOpExpr(8, ctx->tok->type == WCC_TOKEN_BIT_XOR);
}

wccASTNode* wccParsePrecedence10(wccParserCtx* ctx) {
    wccParseBinOpExpr(9, ctx->tok->type == WCC_TOKEN_BIT_OR);
}

wccASTNode* wccParsePrecedence11(wccParserCtx* ctx) {
    wccParseBinOpExpr(10, ctx->tok->type == WCC_TOKEN_AND);
}

wccASTNode* wccParsePrecedence12(wccParserCtx* ctx) {
    wccParseBinOpExpr(11, ctx->tok->type == WCC_TOKEN_OR);
}

wccASTNode* wccParsePrecedence13(wccParserCtx* ctx) {
    // Terenary conditional
    wccASTNode* lhs = wccParsePrecedence12(ctx);
    if (ctx->tok->type == WCC_TOKEN_QUESTION) {
        advance(ctx);
        wccASTNode* rhs = wccParseExpression(ctx);
        wccTernaryNode* node = malloc(sizeof(wccTernaryNode));
        lhs->type = WCC_NODE_TERENARY;
        node->condition = lhs;
        node->exprTrue = rhs;
        if (ctx->tok->type != WCC_TOKEN_COLON) {
            // TODO: Error for no colon
            return NULL;
        }
        node->exprFalse = wccParsePrecedence12(ctx);
    }
    return lhs;
}

wccASTNode* wccParsePrecedence14(wccParserCtx* ctx) {
    wccParseBinOpExpr(13, ctx->tok->type >= WCC_TOKEN_ASSIGN && ctx->tok->type <= WCC_TOKEN_SHR_ASSIGN);
}

wccASTNode* wccParsePrecedence15(wccParserCtx* ctx) {
    wccASTNode* lhs = wccParsePrecedence14(ctx);
    if (ctx->tok->type == WCC_TOKEN_COMMA) {
        wccASTNode** list = malloc(sizeof(wccASTNode*) * 2);
        size_t count = 1;
        list[0] = lhs;
        size_t size = 2;
        while (ctx->tok->type == WCC_TOKEN_COMMA) {
            advance(ctx);
            if (count == size) {
                size *= 2;
                list = realloc(list, sizeof(wccASTNode*) * size);
            }
            list[count++] = wccParsePrecedence14(ctx);
        }
        wccListNode* node = malloc(sizeof(wccListNode));
        node->count = count;
        node->nodes = list;
        lhs->type = WCC_NODE_LIST;
        lhs->node = node;
    }
    return lhs;
}

wccASTNode* wccParseExpression(wccParserCtx* ctx) {
    wccASTNode* node = wccParsePrecedence15(ctx);
    return node;
};

wccASTNode* wccParseStatement(wccParserCtx* ctx) {
    wccASTNode* node = NULL;
    if (ctx -> tok -> type == WCC_TOKEN_KEYWORD) {
        if (!strcmp(ctx->tok->str, "if")) {
            node = malloc(sizeof(wccASTNode));
            size_t cases = 1;
            wccASTNode** bodies = malloc(sizeof(wccASTNode*));
            wccASTNode** conditions = malloc(sizeof(wccASTNode*));
            advance(ctx);
            if (ctx->tok->type != WCC_TOKEN_LPAREN) {
                // TODO: Error for no '('
                return NULL;
            }
            advance(ctx);
            conditions[cases-1] = wccParseExpression(ctx);
            if (conditions[cases-1] == NULL) return NULL;
            if (ctx->tok->type != WCC_TOKEN_RPAREN) {
                // TODO: Error for no ')'
                return NULL;
            }
            advance(ctx);
            bodies[cases-1] = wccParseStatement(ctx);
            if (bodies[cases-1] == NULL) return NULL;
            wccIfNode* ifnode = malloc(sizeof(wccIfNode));
            ifnode->condition = conditions;
            ifnode->body = bodies;
            ifnode->has_else = false;
            while (ctx->tok->type == WCC_TOKEN_KEYWORD && !strcmp(ctx->tok->str, "else")) {
                advance(ctx);
                if (ctx->tok->type == WCC_TOKEN_KEYWORD && !strcmp(ctx->tok->str, "if")) {
                    advance(ctx);
                    if (ctx->tok->type != WCC_TOKEN_LPAREN) {
                        // TODO: Error handling
                        return NULL;
                    }
                    advance(ctx);
                    conditions[cases] = wccParseExpression(ctx);
                    if (conditions[cases] == NULL) return NULL;
                    if (ctx->tok->type != WCC_TOKEN_RPAREN) {
                        // TODO: Error handling
                        return NULL;
                    }
                    advance(ctx);
                    bodies[cases++] = wccParseStatement(ctx);
                    if (bodies[cases-1] == NULL) return NULL;
                } else {
                    ifnode->has_else = true;
                    ifnode->else_body = wccParseStatement(ctx);
                    if (ifnode->else_body == NULL) return NULL;
                }
            }
            ifnode->cases = cases;
            node->type = WCC_NODE_IF;
            node->node = ifnode;
        }
    } else if (ctx->tok->type == WCC_TOKEN_LBRACE) {
        advance(ctx);
        wccCompoundNode* compound = malloc(sizeof(wccCompoundNode));
        compound -> statements = malloc(sizeof(wccASTNode*) * 2),
        compound -> statements_count = 0;
        compound -> line = ctx -> tok -> line;
        compound -> idx = ctx -> tok -> idx;
        compound -> column = ctx -> tok -> col;

        size_t cap = 2;
        bool error = false;

        while (ctx -> tok != NULL && ctx -> tok -> type != WCC_TOKEN_RBRACE) {
            if (compound -> statements_count == cap) {
                cap *= 2;
                compound -> statements = realloc(compound -> statements, sizeof(wccASTNode*) * cap);
            }
            compound -> statements[compound -> statements_count++] = wccParseStatement(ctx);
            if (compound -> statements[compound -> statements_count - 1] == NULL) {
                return NULL;
            }
        }
        node = malloc(sizeof(wccASTNode));
        node->type = WCC_NODE_COMPOUND;
        node->node = compound;
        if (ctx->tok->type != WCC_TOKEN_RBRACE) {
            // TODO: Error
            return NULL;
        }
        advance(ctx);
    }
    if (!node) {
        node = wccParseExpression(ctx);
        if (ctx->tok->type == WCC_TOKEN_SEMICOLON) {
            advance(ctx);
        } else {
            // TODO: Error for no semicolon
            return NULL;
        }
    }
    return node;
}

wccASTNode* wccParseProgram(wccParserCtx* ctx) {
    wccCompoundNode* program = malloc(sizeof(wccCompoundNode));
    program -> statements = malloc(sizeof(wccASTNode*) * 2),
    program -> statements_count = 0;
    program -> line = ctx -> tok -> line;
    program -> idx = ctx -> tok -> idx;
    program -> column = ctx -> tok -> col;

    size_t cap = 2;
    bool error = false;

    while (ctx -> tok != NULL && ctx -> tok -> type != WCC_TOKEN_EOF) {
        if (program -> statements_count == cap) {
            cap *= 2;
            program -> statements = realloc(program -> statements, sizeof(wccASTNode*) * cap);
        }
        program -> statements[program -> statements_count++] = wccParseStatement(ctx);
        if (program -> statements[program -> statements_count - 1] == NULL) {
            error = true;
            advance(ctx);
        }
    }
    wccASTNode* ast = malloc(sizeof(wccASTNode));
    ast -> type = WCC_NODE_COMPOUND;
    ast -> node = program;
    if (error) {
        exit(1); // All errors have been printed, we can now exit
    }
    return ast;
}

/* Debug tools */

char* wccTokenTypeToString(wccToken* op) {
    switch (op->type) {
        case WCC_TOKEN_ADD:    return "+";
        case WCC_TOKEN_SUB:    return "-";
        case WCC_TOKEN_MUL:    return "*";
        case WCC_TOKEN_DIV:    return "/";
        case WCC_TOKEN_MOD:    return "%";
        case WCC_TOKEN_ASSIGN: return "=";
        case WCC_TOKEN_EQ:     return "==";
        default:               return "UNKNOWN";
    }
    return "UNKNOWN";
}

void wccPrintAST(wccASTNode* node, int indent) {
    switch (node->type) {
        case WCC_NODE_BINOP: {
            wccBinOpNode* binop = node->node;
            printf("%*sBinOp: %s\n", indent, "", wccTokenTypeToString(binop->op));
            wccPrintAST(binop->lhs, indent + 4);
            wccPrintAST(binop->rhs, indent + 4);
            break;
        }
        case WCC_NODE_UNOP: {
            wccUnOpNode* unop = node->node;
            printf("%*sUnOp: %s\n", indent, "", wccTokenTypeToString(unop->op));
            wccPrintAST(unop->expr, indent + 4);
            break;
        }
        case WCC_NODE_INT: {
            wccConstNode* integer = node->node;
            printf("%*sInteger: %s\n", indent, "", integer->value->str);
            break;
        }
        case WCC_NODE_VARACCESS: {
            wccConstNode* varaccess = node->node;
            printf("%*sVarAccess: %s\n", indent, "", varaccess->value->str);
            break;
        }
        case WCC_NODE_COMPOUND: {
            wccCompoundNode* compound = node->node;
            printf("%*sCompound:\n", indent, "");
            for (size_t i = 0; i < compound->statements_count; i++) {
                wccPrintAST(compound->statements[i], indent + 4);
            }
            break;
        }
        case WCC_NODE_IF: {
            wccIfNode* ifstatement = node->node;
            printf("%*sIf:\n", indent, "");
            for (size_t i = 0; i < ifstatement->cases; i++) {
                printf("%*sCondition:\n", indent+4, "");
                wccPrintAST(ifstatement->condition[i], indent+8);
                printf("%*sBody:\n", indent+4, "");
                wccPrintAST(ifstatement->body[i], indent+8);
            }
            if (ifstatement->has_else) {
                printf("%*sElse:\n", indent+4, "");
                wccPrintAST(ifstatement->else_body, indent+8);
            }
            break;
        }
        case WCC_NODE_FUNCCALL: {
            wccFuncCallNode* funccall = node->node;
            printf("%*sFunc call:\n", indent, "");
            printf("%*sFunc:\n", indent+4, "");
            wccPrintAST(funccall->func, indent+8);
            printf("%*sArgs:\n", indent+4, "");
            for (size_t i = 0; i < funccall->args->count; i++) {
                wccPrintAST(funccall->args->nodes[i], indent+8);
            }
            if (funccall->args->count == 0) {
                printf("%*s-- No args provided --\n", indent+8, "");
            }
            break;
        }
        default: {
            printf("%*sUnknown (%i)\n", indent, "", node->type);
            break;
        }
    }
}
