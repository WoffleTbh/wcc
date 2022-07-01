/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "wcc/error.h"
#include "token.h"
#include "lexer.h"

wccTokenList* wccTokenize(char* src, char* file) {
    size_t idx = 0;
    size_t line = 1;
    size_t col = 1;
    bool error = false;
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
                col = 1;
                line++;
                break;
            case '+':
                if (src[idx + 1] == '+') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_INC, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ADD_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ADD, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '-':
                if (src[idx + 1] == '-') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DEC, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SUB_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SUB, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '*':
                if (src[idx + 1] == '*') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MUL_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MUL, NULL, line, col, 1, idx));
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
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DIV_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_DIV, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '%':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MOD_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_MOD, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '=':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_EQ, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_ASSIGN, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '!':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_NE, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_NOT, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '<':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LE, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '<') {
                    if (src[idx + 2] == '=') {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHL_ASSIGN, NULL, line, col, 3, idx));
                        idx += 3;
                    } else {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHL, NULL, line, col, 2, idx));
                        idx += 2;
                    }
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LT, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '>':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_GE, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '>') {
                    if (src[idx + 2] == '=') {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHR_ASSIGN, NULL, line, col, 3, idx));
                        idx += 3;
                    } else {
                        wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SHR, NULL, line, col, 2, idx));
                        idx += 2;
                    }
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_GT, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '&':
                if (src[idx + 1] == '&') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_AND, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_AND_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_AND, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '|':
                if (src[idx + 1] == '|') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_OR, NULL, line, col, 2, idx));
                    idx += 2;
                } else if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_OR_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_OR, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '^':
                if (src[idx + 1] == '=') {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_XOR_ASSIGN, NULL, line, col, 2, idx));
                    idx += 2;
                } else {
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_XOR, NULL, line, col, 1, idx));
                    idx++;
                }
                break;
            case '~':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_BIT_NOT, NULL, line, col, 1, idx));
                idx++;
                break;
            case '(':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LPAREN, NULL, line, col, 1, idx));
                idx++;
                break;
            case ')':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RPAREN, NULL, line, col, 1, idx));
                idx++;
                break;
            case '[':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LBRACKET, NULL, line, col, 1, idx));
                idx++;
                break;
            case ']':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RBRACKET, NULL, line, col, 1, idx));
                idx++;
                break;
            case '{':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_LBRACE, NULL, line, col, 1, idx));
                idx++;
                break;
            case '}':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_RBRACE, NULL, line, col, 1, idx));
                idx++;
                break;
            case ',':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_COMMA, NULL, line, col, 1, idx));
                idx++;
                break;
            case ';':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_SEMICOLON, NULL, line, col, 1, idx));
                idx++;
                break;
            case ':':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_COLON, NULL, line, col, 1, idx));
                idx++;
                break;
            case '?':
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_QUESTION, NULL, line, col, 1, idx));
                idx++;
                break;
            case '"': {
                char* value = malloc(2);
                size_t size = 2;
                size_t i = 0;
                size_t start = idx;
                idx++;
                while (src[idx] != '"' && src[idx] != '\n' && src[idx] != 0) {
                    if (src[idx] == '\\') {
                        idx++;
                        switch (src[idx]) {
                            case 'n':
                                value[i] = '\n';
                                break;
                            case 'r':
                                value[i] = '\r';
                                break;
                            case 't':
                                value[i] = '\t';
                                break;
                            case 'b':
                                value[i] = '\b';
                                break;
                            case 'f':
                                value[i] = '\f';
                                break;
                            case '"':
                                value[i] = '"';
                                break;
                            case '\\':
                                value[i] = '\\';
                                break;
                            default:
                                value[i] = src[idx];
                                break;
                        }
                    } else {
                        value[i] = src[idx];
                    }
                    col++;
                    idx++;
                    i++;
                    if (i == size) {
                        size *= 2;
                        value = realloc(value, size);
                    }
                }
                if (src[idx] != '"') {
                    wccLexerError("Unterminated string", file, src, line, col - i, i + 1, start);
                    char* corrected = malloc(i + 3);
                    corrected[0] = '"';
                    memcpy(corrected + 1, value, i);
                    corrected[i + 1] = '"';
                    corrected[i + 2] = 0;
                    wccSuggestion("Append a \" to the end of the string constant", file, src, corrected, line, col - i, i + 1, i + 1, start);

                    error = true;
                }
                if (src[idx] == '\n') {
                    line++;
                    col = 0;
                } else {
                    col += i;
                }
                idx++;
                value[i] = 0;
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_STRING, value, line, col - i, i, start));
                break;
            }
            case '\'': {
                char* value = malloc(2);
                size_t size = 2;
                size_t i = 0;
                size_t start = idx;
                idx++;
                while (src[idx] != '\'' && src[idx] != 0) {
                    if (src[idx] == '\\') {
                        idx++;
                        switch (src[idx]) {
                            case 'n':
                                value[i] = '\n';
                                break;
                            case 'r':
                                value[i] = '\r';
                                break;
                            case 't':
                                value[i] = '\t';
                                break;
                            case 'b':
                                value[i] = '\b';
                                break;
                            case 'f':
                                value[i] = '\f';
                                break;
                            case '\'':
                                value[i] = '\'';
                                break;
                            case '\\':
                                value[i] = '\\';
                                break;
                            default:
                                value[i] = src[idx];
                                break;
                        }
                    } else {
                        value[i] = src[idx];
                    }
                    col++;
                    idx++;
                    i++;
                    if (i == size) {
                        size *= 2;
                        value = realloc(value, size);
                    }
                }
                if (src[idx] != '\'') {
                    wccLexerError("Unterminated character literal", file, src, line, col - i, i, start);
                    char* corrected = malloc(i + 2);
                    corrected[0] = '\'';
                    memcpy(corrected + 1, value, i);
                    corrected[i] = '\'';
                    corrected[i + 1] = 0;
                    wccSuggestion("Append a ' to the end of the character literal", file, src, corrected, line, col - i, i, i, start);
                    error = true;
                }
                idx++;
                value[i] = 0;
                if (i != 1) {
                    wccLexerError("Character literal must be a single character", file, src, line, col - i, i, start);
                    char* corrected = malloc(i + 2);
                    corrected[0] = '"';
                    memcpy(corrected + 1, value, i);
                    corrected[i] = '"';
                    corrected[i + 1] = 0;
                    wccSuggestion("Consider replacing the ' with a \" to make it a string constant", file, src, corrected, line, col - i, i, i, start);

                    error = true;
                }
                wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_CHAR, value, line, col, i, start));
            }
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
                        col++;
                    }
                    value[idx - start] = '\0';
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_IDENTIFIER, value, line, col - (idx - start), idx - start, idx));
                } else if (src[idx] == '0' && (src[idx + 1] == 'x' || src[idx + 1] == 'X')) {
                    idx += 2;
                    char* value = malloc(2);
                    size_t start = idx;
                    size_t size = 2;
                    while ((src[idx] >= '0' && src[idx] <= '9') || (src[idx] >= 'a' && src[idx] <= 'f') || (src[idx] >= 'A' && src[idx] <= 'F') || src[idx] == '_') {
                        if ((idx - start) == size) {
                            size *= 2;
                            value = realloc(value, size);
                        }
                        if (src[idx] == '_') continue;
                        value[idx - start] = src[idx];
                        idx++;
                        col++;
                    }
                    value[idx - start] = 0;
                    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_HEX, value, line, col - (idx - start), idx - start, idx));
                }
                else if (src[idx] >= '0' && src[idx] <= '9') {
                    char* value = malloc(2);
                    size_t start = idx;
                    size_t size = 2;
                    char isFloat = 0;
                    while ((src[idx] >= '0' && src[idx] <= '9') || src[idx] == '.' || src[idx] == '_') {
                        if ((idx - start) == size) {
                            size *= 2;
                            value = realloc(value, size);
                        }
                        if (src[idx] == '.') {
                            isFloat++;
                        }
                        if (src[idx] == '_') continue;
                        value[idx - start] = src[idx];
                        idx++;
                        col++;
                    }
                    value[idx - start] = '\0';
                    if (isFloat > 1) {
                        wccLexerError("Malformed number", file, src, line, col - (idx - start) - 2, idx - start - 1, start);
                        char* corrected = malloc(idx - start + 2);
                        size_t dots = 0;
                        size_t i = 0;
                        size_t j = 0;
                        while (i < idx - start) {
                            if (src[i + start] == '.') {
                                dots++;
                                if (dots == 1) {
                                    corrected[j] = '.';
                                } else {j--;}
                            } else {
                                corrected[j] = src[i + start];
                            }
                            i++;
                            j++;
                        }
                        corrected[idx - start - dots + 1] = 0;
                        wccSuggestion("Consider removing the extra dot(s)", file, src, corrected, line, col - (idx - start) - 2, idx - start - dots, idx - start, start);
                        error = true;
                    }
                    wccTokenList_push(tokens, wccToken_new(isFloat ? WCC_TOKEN_FLOAT : WCC_TOKEN_INT, value, line, col, idx - start, idx));
                } else {
                    idx++;
                }
            }
        }
        col++;
    }
    wccTokenList_push(tokens, wccToken_new(WCC_TOKEN_EOF, NULL, line, col, 0, idx));
    if (error) {
        wccTokenList_free(tokens);
        exit(1);
    }
    return tokens;
}
