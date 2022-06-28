/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include "wcc/error.h"
#include "wcc/term.h"

void wcc_fatal(char* msg) {
    fprintf(stderr, "%sfatal:%s error: %s\n", wcc_red, wcc_style_reset, msg);
    exit(1);
}
