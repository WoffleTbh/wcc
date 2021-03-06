/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once

#include "token.h"

wccTokenList* wccTokenize(char* src, char* file);
