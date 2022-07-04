/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include "wcc/wcc.h"
#include "lexer.h"
#include "token.h"
#include "parser.h"

char* __compile_x86(char* src, char* filename) {
    wccTokenList* tokens = wccTokenize(src, filename);
    wccASTNode* ast = wccParse(src, filename, tokens);
    wccPrintAST(ast, 0);

    wccTokenList_free(tokens);
    return NULL;
}
