/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

char* __compile_x86(char* src, char* filename);
char* __compile_x64(char* src, char* filename);
char* __compile_arm(char* src, char* filename);

#ifdef __x86_64__
    #define wcc_compile(src, filename) __compile_x64(src, filename)
#elif __i386__
    #define wcc_compile(src, filename) __compile_x86(src, filename)
#elif __arm__
    #define wcc_compile(src, filename) __compile_arm(src, filename)
#else
    #error "Unsupported architecture"
#endif

