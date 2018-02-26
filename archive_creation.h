/*Written for God by His grace. */

#ifndef ARCHIVE_CREATION_H
#define ARCHIVE_CREATION_H

#define ASCII_NUM_OFFSET 48

typedef struct {
   char name[100];
   char mode[8];
   char uid[8];
   char gid[8];
   char size[12];
   struct mtime[10];
   unsigned long chksum;
   char typeflag;
   char linkname[100];
   const char *magic;
   const char *version;
   char     *uname;
   char     *gname;
   major_t  devmajor;
   minor_t  devminor;
   char   prefix[155];

} Header;


void traverse_paths(char* pathname);

#endif
