#include "wcc/error.h"
#include "wcc/term.h"

void wcc_fatal(char* msg) {
    fprintf(stderr, "%sfatal:%s error: %s\n", wcc_red, wcc_style_reset, msg);
    exit(1);
}
