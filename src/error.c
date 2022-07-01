/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "wcc/error.h"
#include "wcc/term.h"
#include "token.h"

#define WCC_CHAR_VERTICAL_LINE L'│'
#define WCC_CHAR_T_LINE L'┬'
#define WCC_CHAR_TOP_LEFT L'╭'
#define WCC_CHAR_TOP_RIGHT L'╮'
#define WCC_CHAR_BOTTOM_LEFT L'╰'
#define WCC_CHAR_BOTTOM_RIGHT L'╯'
#define WCC_CHAR_HORIZONTAL_LINE L'─'

void wcc_fatal(char* msg) {
    fprintf(stderr, "%sfatal:%s error: %s\n", wcc_red, wcc_style_reset, msg);
    exit(1);
}

void wccPrintWhitespace(int count) {
    for (int i = 0; i < count; i++) {
        fprintf(stderr, " ");
    }
}

void wccError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx) {
    fprintf(stderr, "%s%s%s:%ld:%ld:%s\n", wcc_style_line, wcc_red, file, line, col, wcc_style_reset);

    col -= 1; // Adjust for column indexing starting at 1

    /* Printing actual code */
    fprintf(stderr, " %ld", line);
    fprintf(stderr, " │ ");

    size_t line_start = idx - col;
    while (line_start < strlen(code) && code[line_start] == '\n') {
        line_start++; // Skip new line(s)
    }
    while (line_start < strlen(code) && code[line_start] != '\n') {
        fprintf(stderr, "%c", code[line_start]);
        line_start++;
    }
    fprintf(stderr, "\n");

    /* Printing underline */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);

    if (len > 1) {
        fprintf(stderr, "╰");
        for (size_t i = 0; i < ((len-3) + (len-3) % 2) / 2; i++) {
            fprintf(stderr, "─");
        }
        fprintf(stderr, "┬");
        for (size_t i = 0; i < ((len + len % 2) - 2) / 2; i++) {
            fprintf(stderr, "─");
        }
        fprintf(stderr, "╯");
    } else {
        fprintf(stderr, "↑");
    }
    fprintf(stderr, "\n");

    /* Printing message */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);
    wccPrintWhitespace(((len-3) + (len-3) % 2) / 2 + 1);
    fprintf(stderr, "│ ");
    fprintf(stderr, "%s%s%s\n", wcc_red, msg, wcc_style_reset);

    /* Printing error line */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);
    wccPrintWhitespace(((len-3) + (len-3) % 2) / 2 + 1);

    fprintf(stderr, "╰");
    for (size_t i = 0; i < strlen(msg) + 2; i++) {
        fprintf(stderr, "─");
    }
    fprintf(stderr, "\n");
}

void wccSuggestion(char* msg, char* file, char* code, char* corrected_token, size_t line, size_t col, size_t len, size_t broken_tok_len, size_t idx) {
    fprintf(stderr, "%s%sSUGGESTION %s:%ld:%ld:%s\n", wcc_style_line, wcc_green, file, line, col, wcc_style_reset);

    col -= 1; // Adjust for column indexing starting at 1

    /* Printing actual code */
    fprintf(stderr, " %ld", line);
    fprintf(stderr, " │ ");

    size_t line_start = idx - col;
    while (line_start < strlen(code) && code[line_start] == '\n') {
        line_start++; // Skip new line(s)
    }
    while (line_start < strlen(code) && code[line_start] != '\n') {
        if (line_start < col || line_start >= col + broken_tok_len) {
            fprintf(stderr, "%c", code[line_start]);
        } else {
            fprintf(stderr, "%s%c%s", wcc_green, line_start - col < strlen(corrected_token) ? corrected_token[line_start - col] : '\0', wcc_style_reset);
        }
        line_start++;
    }
    fprintf(stderr, "\n");

    /* Printing underline */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);

    if (len > 1) {
        fprintf(stderr, "╰");
        for (size_t i = 0; i < ((len-3) + (len-3) % 2) / 2; i++) {
            fprintf(stderr, "─");
        }
        fprintf(stderr, "┬");
        for (size_t i = 0; i < ((len + len % 2) - 2) / 2; i++) {
            fprintf(stderr, "─");
        }
        fprintf(stderr, "╯");
    } else {
        fprintf(stderr, "↑");
    }
    fprintf(stderr, "\n");

    /* Printing message */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);
    wccPrintWhitespace(((len-3) + (len-3) % 2) / 2 + 1);
    fprintf(stderr, "│ ");
    fprintf(stderr, "%s%s%s\n", wcc_green, msg, wcc_style_reset);

    /* Printing suggestion line */
    wccPrintWhitespace(log10(line) + 2);
    fprintf(stderr, " │ ");
    wccPrintWhitespace(col);
    wccPrintWhitespace(((len-3) + (len-3) % 2) / 2 + 1);

    fprintf(stderr, "╰");
    for (size_t i = 0; i < strlen(msg) + 2; i++) {
        fprintf(stderr, "─");
    }
    fprintf(stderr, "\n");
}


void wccLexerError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx) {
    wccError(msg, file, code, line, col, len, idx);
}

void _wccParserError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx) {
    wccError(msg, file, code, line, col, len, idx);
}

void wccParserError(char* msg, char* file, char* code, wccToken* token) {
    _wccParserError(msg, file, code, token->line, token->col, token->len, token->idx);
}
