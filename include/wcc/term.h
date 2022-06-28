/*
 * This file is part of the Woffle C Compiler
 * See LICENSE for license
 * See README.md for more info about the project
 * */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define wcc_black         "\033[30m"
#define wcc_red           "\033[31m"
#define wcc_green         "\033[32m"
#define wcc_yellow        "\033[33m"
#define wcc_blue          "\033[34m"
#define wcc_magenta       "\033[35m"
#define wcc_cyan          "\033[36m"
#define wcc_white         "\033[37m"
#define wcc_rgb(r,g,b)    "\033[38;2;" #r ";" #g ";" #b "m"

#define wcc_bg_black      "\033[40m"
#define wcc_bg_red        "\033[41m"
#define wcc_bg_green      "\033[42m"
#define wcc_bg_yellow     "\033[43m"
#define wcc_bg_blue       "\033[44m"
#define wcc_bg_magenta    "\033[45m"
#define wcc_bg_cyan       "\033[46m"
#define wcc_bg_white      "\033[47m"
#define wcc_bg_rgb(r,g,b) "\033[48;2;" #r ";" #g ";" #b "m"

#define wcc_style_reset   "\033[0m"
#define wcc_style_bold    "\033[1m"
#define wcc_style_dim     "\033[2m"
#define wcc_style_italic  "\033[3m"
#define wcc_style_line    "\033[4m"
#define wcc_style_blinks  "\033[5m"
#define wcc_style_invert  "\033[7m"
#define wcc_style_invis   "\033[8m"
#define wcc_style_strike  "\033[9m"

#define wcc_clear         "\033[2J"
#define wcc_clearln       "\033[2K"

#ifdef __cplusplus
}
#endif

