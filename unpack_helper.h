/*Written for God by his infinite grace. */
#ifndef UNPACK_HELPER_H
#define UNPACK_HELPER_H


#define ASCII_NUM_OFFSET 48
#define BLOCK_SIZE 512

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "math.h"

#include "directory_struct.h"


long octal_to_decimal(char* octal_char, int size);
void store_gname(char *, D_Node *node);
void store_uname(char*, D_Node *node);
void store_typeflag(char*, D_Node *node);
void store_mtime(char*, D_Node *node);
void store_size(char *, D_Node *node);
void store_gid(char*, D_Node *node);
void store_uid(char*, D_Node *node);
void store_mode(char*, D_Node *node);
void store_name(char*, D_Node *node);
void store_one(char*, D_Node *node);
double pow(double a, double b);

#endif
