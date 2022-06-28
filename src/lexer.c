/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdlib.h>
#include "token.h"
#include "lexer.h"

wccTokenList* wccTokenize(char* src) {
    size_t idx = 0;
    size_t line = 1;
    size_t col = 1;
    wccTokenList* tokens = wccTokenList_new();
    while (src[idx] != 0) {
        switch (src[idx]) {
            case ' ':
            case '\t':
            case '\r':
                idx++;
                break;
            case '\n':
                idx++;
                line++;
                break;
            case '+':
                if (src[idx + 1] == '+') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_INC, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ADD_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ADD, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '-':
                if (src[idx + 1] == '-') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DEC, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SUB_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SUB, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '*':
                if (src[idx + 1] == '*') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MUL_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MUL, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '/':
                if (src[idx + 1] == '/') {
                    idx += 2;
                    while(src[idx] != '\n' && src[idx] != 0) {
                        idx++;
                    }
                } else if (src[idx + 1] == '*') {
                    idx += 2;
                    while(src[idx] != '*' && src[idx + 1] != '/' && src[idx] != 0) {
                        idx++;
                    }
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DIV_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DIV, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '=':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_EQ, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ASSIGN, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '!':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_NE, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_NOT, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '<':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LE, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '<') {
                    if (src[idx + 2] == '=') {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHL_ASSIGN, NULL, line, col, 3));
                        idx += 3;
                    } else {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHL, NULL, line, col, 2));
                        idx += 2;
                    }
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LT, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '>':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_GE, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '>') {
                    if (src[idx + 2] == '=') {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHR_ASSIGN, NULL, line, col, 3));
                        idx += 3;
                    } else {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHR, NULL, line, col, 2));
                        idx += 2;
                    }
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_GT, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '&':
                if (src[idx + 1] == '&') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_AND, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_AND_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_AND, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '|':
                if (src[idx + 1] == '|') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_OR, NULL, line, col, 2));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_OR_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_OR, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '^':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_XOR_ASSIGN, NULL, line, col, 2));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_XOR, NULL, line, col, 1));
                    idx++;
                }
                break;
            case '~':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_NOT, NULL, line, col, 1));
                idx++;
                break;
            case '(':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LPAREN, NULL, line, col, 1));
                idx++;
                break;
            case ')':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RPAREN, NULL, line, col, 1));
                idx++;
                break;
            case '[':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LBRACKET, NULL, line, col, 1));
                idx++;
                break;
            case ']':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RBRACKET, NULL, line, col, 1));
                idx++;
                break;
            case '{':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LBRACE, NULL, line, col, 1));
                idx++;
                break;
            case '}':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RBRACE, NULL, line, col, 1));
                idx++;
                break;
            case ',':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_COMMA, NULL, line, col, 1));
                idx++;
                break;
            case ';':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SEMICOLON, NULL, line, col, 1));
                idx++;
                break;
            case ':':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_COLON, NULL, line, col, 1));
                idx++;
                break;
            case '?':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_QUESTION, NULL, line, col, 1));
                idx++;
                break;
            default: {
                if ((src[idx] >= 'a' && src[idx] <= 'z') || (src[idx] >= 'A' && src[idx] <= 'Z') || src[idx] == '_') {
                    char* value = malloc(2);
                    size_t start = idx;
                    size_t size = 2;
                    while ((src[idx] >= 'a' && src[idx] <= 'z') || (src[idx] >= 'A' && src[idx] <= 'Z') || (src[idx] >= '0' && src[idx] <= '9') || src[idx] == '_') {
                        if ((idx - start) == size) {
                            size *= 2;
                            value = realloc(value, size);
                        }
                        value[idx - start] = src[idx];
                        idx++;
                    }
                    value[idx - start] = '\0';
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_IDENTIFIER, value, line, col, idx - start));
                } else if (src[idx] >= '0' && src[idx] <= '9') { // TODO: Handle binary, hex, and octal numbers
                    char* value = malloc(2);
                    size_t start = idx;
                    size_t size = 2;
                    char isFloat = 0;
                    while (src[idx] >= '0' && src[idx] <= '9') {
                        if ((idx - start) == size) {
                            size *= 2;
                            value = realloc(value, size);
                        }
                        if (src[idx] == '.') {
                            isFloat = 1;
                        }
                        value[idx - start] = src[idx];
                        idx++;
                    }
                    value[idx - start] = '\0';
                    wccTokenList_push(tokens, wccToken_new(isFloat ? WCC_TOKEN_FLOAT : WCC_TOKEN_INT, value, line, col, idx - start));
                }
            }
        }
    }
    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_EOF, NULL, line, col, 0));
    return tokens;
}
