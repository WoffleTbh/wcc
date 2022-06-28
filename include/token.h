/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

#include <stdint.h>

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
    WCC_TOKEN_SAR,

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
    WCC_TOKEN_SAR_ASSIGN,

    /* Increment/Decrement */
    WCC_TOKEN_POST_INC,
    WCC_TOKEN_POST_DEC,
    WCC_TOKEN_PRE_INC,
    WCC_TOKEN_PRE_DEC,

    /* Values */
    WCC_TOKEN_INT,
    WCC_TOKEN_FLOAT,
    WCC_TOKEN_COMPLEX,
    WCC_TOKEN_BINARY,
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
    WCC_TOKEN_EOF,
} wccTokenType;

typedef struct {
    wccTokenType type;
    char* str;
    size_t line;
    size_t col;
    size_t len;
} wccToken;

typedef struct {
    wccToken** tokens;
    size_t size;
    size_t capacity;
} wccTokenList;

inline wccToken* wccToken_new(wccTokenType type, char* str, size_t line, size_t col, size_t len);
inline void wccToken_free(wccToken* token);

inline wccTokenList* wccTokenList_create();
inline void wccTokenList_destroy(wccTokenList* list);
void wccTokenList_push(wccTokenList* list, wccToken* token);

