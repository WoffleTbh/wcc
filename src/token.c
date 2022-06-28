/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#include <stdlib.h>
#include "wcc/error.h"
#include "token.h"

inline wccToken* wccToken_new(wccTokenType type, char* str, size_t line, size_t col, size_t len) {
    wccToken* token = malloc(sizeof(wccToken));
    token->type = type;
    token->str = str;
    token->line = line;
    token->col = col;
    token->len = len;
    return token;
}

inline void wccToken_free(wccToken* token) {
    free(token->str);
    free(token);
}

inline wccTokenList* wccTokenList_create() {
    wccTokenList* list = malloc(sizeof(wccTokenList));
    list -> tokens = malloc(sizeof(wccToken*) * WCC_TOKEN_LIST_INITIAL_CAPACITY);
    list -> capacity = WCC_TOKEN_LIST_INITIAL_CAPACITY;
    list -> size = 0;
    return list;
}

inline void wccTokenList_destroy(wccTokenList* list) {
    for (size_t i = 0; i < list -> size; i++) {
        wccToken_free(list -> tokens[i]);
    }
    free(list -> tokens);
    free(list);
}

void wccTokenList_push(wccTokenList* list, wccToken* token) {
    if (list -> size == list -> capacity) {
        list -> capacity *= 2;
        list -> tokens = realloc(list -> tokens, sizeof(wccToken*) * list -> capacity);
        if (list -> tokens == NULL) {
            wcc_fatal("could not allocate memory for token list");
        }
    }
    list -> tokens[list -> size++] = token;
}
