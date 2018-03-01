/*Written for God by His grace. */

#ifndef ARCHIVE_CREATION_H
#define ARCHIVE_CREATION_H

/*#define _BSD_SOURCE*/

#define ASCII_NUM_OFFSET 48
#define OFF_SET 2
#define USTAR_ASCII_SUM 559
#define VERSION_ASCII_SUM 96
#define BLOCK_SIZE 512
#define INVALID_READ 0
#define MAX_OCTAL_IN_DECIMAL 2097151
#define CHKSUM_OFFSET 224

#define NAME_SIZE 100
#define MODE_LEN 8
#define UID_LEN 8
#define GID_LEN 8
#define SIZE_LEN 12
#define MTIME_LEN 12
#define CHKSUM_LEN 8
#define TYPEFLAG_LEN 1
#define LINKNAME_LEN 100
#define MAGIC_LEN 6
#define VERSION_LEN 2
#define UNAME_LEN 32
#define GNAME_LEN 32
#define DEVMAJOR_LEN 8
#define DEVMINOR_LEN 8
#define PREFIX_LEN 155


#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <arpa/inet.h>

typedef struct {
   char name[100];
   char mode[8];
   char uid[8];
   char gid[8];
   char size[12];
   char mtime[12];
   char chksum[8];
   char typeflag;
   char linkname[100];
   char magic[6];
   char version[2];
   char     uname[32];
   char     gname[32];
   char  devmajor[8];
   char  devminor[8];
   char   prefix[155];
   int uidflag;
   int gidflag;
} Header;


void write_header(Header * header, int fd);
Header *create_header(char * path);
void traverse_paths(char* pathname);
void write_entry(char *path, char *tarfile, int rfd, int wfd);
static void shove_it_in(char *buffer, char *field, int index, int size);
#endif
