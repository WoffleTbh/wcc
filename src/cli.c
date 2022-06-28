/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wcc/error.h"
#include "wcc/wcc.h"

#define WCC_MAX_SOURCE_FILES 10

char* OUTPUT_FILE = NULL;
char* SOURCE_FILES[WCC_MAX_SOURCE_FILES] = {NULL};

void parse_args(int argc, char** argv) {
    int source_file_count = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            if (i == argc) {
                wcc_fatal("missing output file");
                exit(1);
            }
            OUTPUT_FILE = argv[++i];
        } else {
            if (i == argc) {
                wcc_fatal("missing source file");
                exit(1);
            }
            SOURCE_FILES[source_file_count++] = argv[i];
        }
    }
}

void compile_file(char* file) {
    FILE* source = fopen(file, "r");
    if (source == NULL) {
        wcc_fatal("could not open source file");
        exit(1);
    }
    char* source_buffer = malloc(sizeof(char) * 1024);
    size_t source_buffer_size = 1024;
    size_t source_buffer_len = 0;
    char c;
    do {
        c = fgetc(source);
        if (c == EOF) {
            break;
        }
        source_buffer[source_buffer_len++] = c;
        if (source_buffer_len == source_buffer_size) {
            source_buffer_size *= 2;
            source_buffer = realloc(source_buffer, source_buffer_size);
        }
    } while (c != EOF);
    fclose(source);
    
    source_buffer[source_buffer_len] = 0;

    wcc_compile(source_buffer);
}
int main(int argc, char** argv) {
    parse_args(argc, argv);
    for (int i = 0; i < WCC_MAX_SOURCE_FILES; i++) {
        if (SOURCE_FILES[i] != NULL) {
            compile_file(SOURCE_FILES[i]);
        }
    }
}
