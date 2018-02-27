/*Written for God by His grace. */

#ifndef ARCHIVE_CREATION_H
#define ARCHIVE_CREATION_H

#define ASCII_NUM_OFFSET 48
#define OFF_SET 2
#define USTAR_ASCII_SUM 559
#define VERSION_ASCII_SUM 96

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>


typedef struct {
   char name[100];
   char mode[8];
   char uid[8];
   char gid[8];
   char size[12];
   char mtime[10];
   unsigned long chksum;
   char typeflag;
   char linkname[100];
   const char magic[6];
   const char version[3];
   char     uname[32];
   char     gname[32];
   char  devmajor[8];
   char  devminor[8];
   char   prefix[155];

} Header;


void traverse_paths(char* pathname);

#endif
