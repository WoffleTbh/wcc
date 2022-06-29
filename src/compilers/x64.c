/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include "wcc/wcc.h"
#include "lexer.h"
#include "token.h"

char* __compile_x64(char* src, char* filename) {
    return __compile_x86(src, filename);
}
