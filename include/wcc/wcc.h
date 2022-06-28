/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

char* __compile_x86(char* src);
char* __compile_x64(char* src);
char* __compile_arm(char* src);

#ifdef __x86_64__
    #define wcc_compile(src) __compile_x64(src)
#elif __i386__
    #define wcc_compile(src) __compile_x86(src)
#elif __arm__
    #define wcc_compile(src) __compile_arm(src)
#else
    #error "Unsupported architecture"
#endif

