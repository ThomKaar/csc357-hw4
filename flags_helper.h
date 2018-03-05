/* Written for God by His grace. */
#ifndef  FLAGS_HELPER_H
#define FLAGS_HELPER_H

#define FLAGS_INDEX 1
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int flags_has_f(char *argv[]);
int flags_incorrect_length(char *argv[]);
int flags_has_x(char *argv[]);
int flags_has_t(char *argv[]);
int flags_has_v(char *argv[]);
#endif
