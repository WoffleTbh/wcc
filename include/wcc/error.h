/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "wcc/term.h"
#include "token.h"

/* cli errors; undetailed */
void wcc_fatal(char* msg);

/* Errors with token pointing */
void wccSuggestion(char* msg, char* file, char* code, char* corrected_token, size_t line, size_t col, size_t len, size_t broken_tok_len, size_t idx);
void wccLexerError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx);
void _wccParserError(char* msg, char* file, char* code, size_t line, size_t col, size_t len, size_t idx);
void wccParserError(char* msg, char* file, char* code, wccToken* token); // token contains all positional data

