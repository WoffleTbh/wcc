/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdlib.h>
#include <string.h>
#include "wcc/error.h"
#include "token.h"

char* WCC_C_KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    "__attribute__", "__asm", "__asm__"
};

wccToken* wccToken_new(wccTokenType type, char* str, size_t line, size_t col, size_t len, size_t idx) {
    wccToken* token = malloc(sizeof(wccToken));
    token->type = type;
    token->str = str;
    token->line = line;
    token->col = col;
    token->len = len;
    token->idx = idx;
    if (str != NULL) {
        for (size_t i = 0; i < sizeof(WCC_C_KEYWORDS) / sizeof(char*); i++) {
            if (strcmp(WCC_C_KEYWORDS[i], str) == 0) {
                token->type = WCC_TOKEN_KEYWORD;
                break;
            }
        }
    }

    return token;
}

void wccToken_free(wccToken* token) {
    free(token->str);
    free(token);
}

wccTokenList* wccTokenList_new() {
    wccTokenList* list = malloc(sizeof(wccTokenList));
    list -> tokens = malloc(sizeof(wccToken*) * WCC_TOKEN_LIST_INITIAL_CAPACITY);
    list -> capacity = WCC_TOKEN_LIST_INITIAL_CAPACITY;
    list -> size = 0;
    return list;
}

void wccTokenList_free(wccTokenList* list) {
    for (size_t i = 0; i < list -> size; i++) {
        wccToken_free(list -> tokens[i]);
    }
    free(list -> tokens);
    free(list);
}

void wccTokenList_push(wccTokenList* list, wccToken* token) {
    if (list -> size == list -> capacity) {
        list -> capacity *= 2;
        list -> tokens = realloc(list -> tokens, sizeof(wccToken*) * list -> capacity);
        if (list -> tokens == NULL) {
            wcc_fatal("could not allocate memory for token list");
        }
    }
    list -> tokens[list -> size++] = token;
}

char* wccTokenNameFromType(wccTokenType type) {
    switch (type) {
        case WCC_TOKEN_ADD:
            return "add";
        case WCC_TOKEN_SUB:
            return "sub";
        case WCC_TOKEN_MUL:
            return "mul";
        case WCC_TOKEN_DIV:
            return "div";
        case WCC_TOKEN_MOD:
            return "mod";
        case WCC_TOKEN_AND:
            return "and";
        case WCC_TOKEN_OR:
            return "or";
        case WCC_TOKEN_XOR:
            return "xor";
        case WCC_TOKEN_NOT:
            return "not";
        case WCC_TOKEN_SHL:
            return "shl";
        case WCC_TOKEN_SHR:
            return "shr";
        case WCC_TOKEN_BIT_AND:
            return "bit_and";
        case WCC_TOKEN_BIT_OR:
            return "bit_or";
        case WCC_TOKEN_BIT_XOR:
            return "bit_xor";
        case WCC_TOKEN_BIT_NOT:
            return "bit_not";
        case WCC_TOKEN_EQ:
            return "eq";
        case WCC_TOKEN_NE:
            return "ne";
        case WCC_TOKEN_LE:
            return "le";
        case WCC_TOKEN_GE:
            return "ge";
        case WCC_TOKEN_LT:
            return "lt";
        case WCC_TOKEN_GT:
            return "gt";
        case WCC_TOKEN_ASSIGN:
            return "assign";
        case WCC_TOKEN_ADD_ASSIGN:
            return "add_assign";
        case WCC_TOKEN_SUB_ASSIGN:
            return "sub_assign";
        case WCC_TOKEN_MUL_ASSIGN:
            return "mul_assign";
        case WCC_TOKEN_DIV_ASSIGN:
            return "div_assign";
        case WCC_TOKEN_MOD_ASSIGN:
            return "mod_assign";
        case WCC_TOKEN_AND_ASSIGN:
            return "and_assign";
        case WCC_TOKEN_OR_ASSIGN:
            return "or_assign";
        case WCC_TOKEN_XOR_ASSIGN:
            return "xor_assign";
        case WCC_TOKEN_SHL_ASSIGN:
            return "shl_assign";
        case WCC_TOKEN_SHR_ASSIGN:
            return "shr_assign";
        case WCC_TOKEN_INC:
            return "inc";
        case WCC_TOKEN_DEC:
            return "dec";
        case WCC_TOKEN_INT:
            return "int";
        case WCC_TOKEN_FLOAT:
            return "float";
        case WCC_TOKEN_CHAR:
            return "char";
        case WCC_TOKEN_STRING:
            return "string";
        case WCC_TOKEN_IDENTIFIER:
            return "identifier";
        case WCC_TOKEN_KEYWORD:
            return "keyword";
        case WCC_TOKEN_LPAREN:
            return "lparen";
        case WCC_TOKEN_RPAREN:
            return "rparen";
        case WCC_TOKEN_LBRACE:
            return "lbrace";
        case WCC_TOKEN_RBRACE:
            return "rbrace";
        case WCC_TOKEN_LBRACKET:
            return "lbracket";
        case WCC_TOKEN_RBRACKET:
            return "rbracket";
        case WCC_TOKEN_COMMA:
            return "comma";
        case WCC_TOKEN_SEMICOLON:
            return "semicolon";
        case WCC_TOKEN_COLON:
            return "colon";
        case WCC_TOKEN_DOT:
            return "dot";
        case WCC_TOKEN_QUESTION:
            return "question";
        case WCC_TOKEN_EOF:
            return "eof";
        default:
            return "unhandled";
    }
    return "unknown";
}
