/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include <stdlib.h>
#include "wcc/wcc.h"
#include "wcc/term.h"

char* __compile_x86(char* src) {

}

char* __compile_x64(char* src) {
    return __compile_x86(src); // TODO: implement x64 compiler, for now just use x86 as it's runnable on both x86 and x64
}

char* __compile_arm(char* src) {
    fprintf(stderr, "%sfatal:%s arm not yet supported\n", wcc_red, wcc_style_reset);
    exit(1);
}
