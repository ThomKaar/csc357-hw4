/*Written for God by his infinite grace. */
#ifndef UNPACK_HELPER_H
#define UNPACK_HELPER_H

#define ASCII_NUM_OFFSET 48

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "math.h"

#include "directory_struct.h"

long octal_to_decimal(char* octal_char, int size);
void store_gname(int rfd, D_Node *node);
void store_uname(int rfd, D_Node *node);
void store_typeflag(int rfd, D_Node *node);
void store_mtime(int rfd, D_Node *node);
void store_size(int rfd, D_Node *node);
void store_gid(int rfd, D_Node *node);
void store_uid(int rfd, D_Node *node);
void store_mode(int rfd, D_Node *node);
void store_name(int rfd, D_Node *node);

#endif
