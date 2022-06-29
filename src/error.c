/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include <string.h>
#include "wcc/error.h"
#include "wcc/term.h"
#include "token.h"

void wcc_fatal(char* msg) {
    fprintf(stderr, "%sfatal:%s error: %s\n", wcc_red, wcc_style_reset, msg);
    exit(1);
}

void wccLexerError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx) {
    fprintf(stderr, "%s%s%s:%ld:%ld:%s\n", wcc_red, wcc_style_line, file, line, col, wcc_style_reset);
    fprintf(stderr, "% 4ld | ", line);
    col -= 1;
    size_t line_start = idx - col;
    while (line_start < strlen(code) && code[line_start] != '\n') {
        fprintf(stderr, "%c", code[line_start]);
        line_start++;
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "     | ");
    for (size_t i = 0; i < col; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, len > 1 ? "├" : "│");
    for (size_t i = 1; i < len - 1; i++) {
        fprintf(stderr, "─");
    }
    if (len > 1) {
        fprintf(stderr, "╯");
    }
    fprintf(stderr, "\n     | ");
    for (size_t i = 0; i < col; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "│ ");
    fprintf(stderr, "%s%s%s\n", wcc_red, msg, wcc_style_reset);
    fprintf(stderr, "     | ");
    for (size_t i = 0; i < col; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "╰");
    for (size_t i = 1; i < strlen(msg) + 2; i++) {
        fprintf(stderr, "─");
    }
    fprintf(stderr, ">\n");
    fprintf(stderr, "\n");
    exit(1);
}
