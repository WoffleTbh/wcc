/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

#include <stdlib.h>

#define WCC_TOKEN_LIST_INITIAL_CAPACITY 32

typedef enum {
    /* Arithmetic */
    WCC_TOKEN_ADD,
    WCC_TOKEN_SUB,
    WCC_TOKEN_MUL,
    WCC_TOKEN_DIV,
    WCC_TOKEN_MOD,

    /* Bitwise */
    WCC_TOKEN_AND,
    WCC_TOKEN_OR,
    WCC_TOKEN_XOR,
    WCC_TOKEN_NOT,
    WCC_TOKEN_SHL,
    WCC_TOKEN_SHR,
    WCC_TOKEN_BIT_AND,
    WCC_TOKEN_BIT_OR,
    WCC_TOKEN_BIT_XOR,
    WCC_TOKEN_BIT_NOT,

    /* Comparison */
    WCC_TOKEN_EQ,
    WCC_TOKEN_NE,
    WCC_TOKEN_LT,
    WCC_TOKEN_GT,
    WCC_TOKEN_LE,
    WCC_TOKEN_GE,

    /* Assignment */
    WCC_TOKEN_ASSIGN,
    WCC_TOKEN_ADD_ASSIGN,
    WCC_TOKEN_SUB_ASSIGN,
    WCC_TOKEN_MUL_ASSIGN,
    WCC_TOKEN_DIV_ASSIGN,
    WCC_TOKEN_MOD_ASSIGN,
    WCC_TOKEN_AND_ASSIGN,
    WCC_TOKEN_OR_ASSIGN,
    WCC_TOKEN_XOR_ASSIGN,
    WCC_TOKEN_SHL_ASSIGN,
    WCC_TOKEN_SHR_ASSIGN,

    /* Increment/Decrement */
    WCC_TOKEN_INC,
    WCC_TOKEN_DEC,

    /* Values */
    WCC_TOKEN_INT,
    WCC_TOKEN_FLOAT,
    WCC_TOKEN_COMPLEX,
    WCC_TOKEN_BIN,
    WCC_TOKEN_HEX,
    WCC_TOKEN_OCT,
    WCC_TOKEN_CHAR,
    WCC_TOKEN_STRING,
    WCC_TOKEN_IDENTIFIER,
    WCC_TOKEN_KEYWORD,

    /* Parenthises */
    WCC_TOKEN_LPAREN,
    WCC_TOKEN_RPAREN,
    WCC_TOKEN_LBRACE,
    WCC_TOKEN_RBRACE,
    WCC_TOKEN_LBRACKET,
    WCC_TOKEN_RBRACKET,

    /* Delimiters */
    WCC_TOKEN_COMMA,
    WCC_TOKEN_SEMICOLON,
    WCC_TOKEN_COLON,
    WCC_TOKEN_DOT,

    /* Misc */
    WCC_TOKEN_QUESTION,
    WCC_TOKEN_EOF,
} wccTokenType;

typedef struct {
    wccTokenType type;
    char* str;
    size_t line;
    size_t col;
    size_t len;
    size_t idx;
} wccToken;

typedef struct {
    wccToken** tokens;
    size_t size;
    size_t capacity;
} wccTokenList;

wccToken* wccToken_new(wccTokenType type, char* str, size_t line, size_t col, size_t len, size_t idx);
void wccToken_free(wccToken* token);

wccTokenList* wccTokenList_new();
void wccTokenList_free(wccTokenList* list);
void wccTokenList_push(wccTokenList* list, wccToken* token);

char* wccTokenNameFromType(wccTokenType type);
