#ifndef TABLE_H
#define TABLE_H

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "unpack_helper.h"
#include "directory_struct.h"

void print_table(int fd, char option);
void print_file_info(D_Node *node);
void print_filename(D_Node * node);

#endif
