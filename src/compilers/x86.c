/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include "wcc/wcc.h"
#include "lexer.h"
#include "token.h"

char* __compile_x86(char* src, char* filename) {
    wccTokenList* tokens = wccTokenize(src, filename);
    for (size_t i = 0; i < tokens -> size; i++) {
        wccToken* token = tokens -> tokens[i];
        printf("%s", wccTokenNameFromType(token -> type));
        if (token -> str != NULL) {
            printf(" %s", token -> str);
        }
        printf("\n");
    }
    printf("\n");
    wccTokenList_free(tokens);
    return NULL;
}
