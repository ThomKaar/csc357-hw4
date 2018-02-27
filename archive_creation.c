/*Written for God by His grace. */

#include "archive_creation.h"

/*This file's purpose is to store helper functions to
 Archive files for mytar. */

static void clear_char_array(char *array, int size){
  int i;
  for(i=0; i< size; i++){
    array[i] = '\0';
  }
}

static void header_set_name(Header* header, char* name){
  strcpy(header->name, name);
}

static void header_set_prefix(Header* header, char* prefix_name){
  strcpy(header->prefix, prefix_name);
}

/*given: the path (aquired by getpwd()), a char array of 100, and a char array size 155*/
static void prefix_name_split(char*path, char*d_name, char*name, char* prefix){
  int offset;

  if(strlen(d_name) > 100){
    offset = strlen(d_name) - 100;
    strcpy(name, (d_name + offset));

    strcpy(prefix, path);
    strncat(prefix, d_name, offset);
  }
  else{
    strncpy(name, d_name, strlen(d_name));
    strcpy(prefix, path);
  }
}

/*Add a string representation of mode_t as the mode member of header
bitwise or the st_mode with the mode #defines */
static void header_set_mode(Header *header, mode_t st_mode){
    char mode[8];
    int i;

    clear_char_array(mode, 8);
    i = 0;
    while(i < 7){
      mode[7-i] = (char) ((st_mode % 8) + ASCII_NUM_OFFSET);
      st_mode /= 8;
      i++;
    }
    mode[7] = '\0';
    strcpy(header->mode, mode);
}


/*Given a uid in octal create a string representing the octal values*/
static void header_set_uid(Header *header, uid_t uid){
  char u[8];
  int i;

  clear_char_array(u,8);

  i = 0;
  while(i < 7){
    u[7-i] = (char) ((uid % 8) + ASCII_NUM_OFFSET);
    uid /= 8;
    i++;
  }
  u[7] = '\0';
  strcpy(header->uid, u);
}

static void header_set_gid(Header *header, gid_t gid){
  char g[8];
  int i;

  clear_char_array(g, 8);

  i = 0;
  while(i < 7){
    g[7-i] = (char) ((gid % 8) + ASCII_NUM_OFFSET);
    gid /= 8;
    i++;
  }
  g[7] = '\0'; /* Changed from 8 to 7 to be within range */
  strcpy(header->gid, g);
}

/**/
static void header_set_size(Header *header, off_t size, mode_t mode){
  char header_size[12];
  int i;
  for(i=0;i<12;i++){
    header_size[i] = '0';
  }
  header_size[11] = '\0';
  i = 0;
  if(!S_ISREG(mode)){
    strcpy(header->size, header_size);
  }
  else{
    while(i < 11){
      header_size[11-i] = (char) ((size % 8) + ASCII_NUM_OFFSET);
      size /= 8;
      i++;
    }
    header_size[11] = '\0';
    strcpy(header->gid, header_size);
  }
}

static void header_set_mtime(Header *header, time_t timespec){
  char tim[12]; /*I'm pretty sure <time> is a keyword in C, changed to tim */
  int i;
  clear_char_array(tim, 12);
  i = 0;
  while(i < 11){
    tim[11-i] = (char) ((timespec % 8) + ASCII_NUM_OFFSET);
    timespec /= 8;
    i++;
  }
  tim[11] = '\0'; /* Changed index from 12 to 11, to be in correct range */
  strcpy(header->mtime, tim);
  return ;
}

/*TODO: Still might need to implement the regular file (alternate)*/
static void header_set_typeflag(Header *header, mode_t mode){
  if(S_ISREG(mode)){
    header->typeflag = '0';
  }
  else if(S_ISLINK(mode)){
    header->typeflag = '2';
  }
  else if(S_ISDIR(mode)){
    header->typeflag = '5';
  }
}

static void header_set_linkname(Header * header, mode_t mode, char* linkname){
  char namebuffer[100];
  clear_char_array(namebuffer, 100);
  if(S_ISLINK(mode)){
    strcpy(header->linkname, linkname);
  }
  else{
    strcpy(header->linkname, namebuffer);
  }
}

static void header_compute_chksum(Header *header, char* field){
   int i;
   i = 0; /* initialized i here --A */
   while(field[i] != '\0')
   {
      header->chksum += field[i];
      i++;
   }
   return;
}

static void header_set_uname(Header* header, uid_t uid){
  struct passwd *pw;
  pw = getpwuid(uid);
  strcpy(header->uname, pw->pw_name);
}

static void header_set_gname(Header* header, gid_t gid){
  struct group *gr;
  gr = getgrgid(gid);
  strcpy(header->gname, gr->gr_name);
}

static void header_set_chksum(Header *header)
{
   header_compute_chksum(header, header->devminor);
   header_compute_chksum(header, header->devmajor);
   header->chksum += USTAR_ASCII_SUM;
   header->chksum += VERSION_ASCII_SUM;
   header_compute_chksum(header, header->gname);
   header_compute_chksum(header, header->uname);
   header_compute_chksum(header, header->linkname);
   header->chksum += header->typeflag;
   header_compute_chksum(header, header->mtime);
   header_compute_chksum(header, header->size);
   header_compute_chksum(header, header->name);
   header_compute_chksum(header, header->mode);
   header_compute_chksum(header, header->uid);
   header_compute_chksum(header, header->gid);
   header_compute_chksum(header, header->prefix);
   return;
}

Header * create_header(char * path, struct dirent* direntp){
  char name[100];
  char prefix[155];
  struct stat *sb;
  Header * header;
  header->chksum = 0;


  lstat(path, sb);

  /*Check this, but devminor and major are length 8
   * so I will make them char arrays of size 8*/
  clear_char_array(header->devminor, 8);
  clear_char_array(header->devmajor, 8);

  /*header->devminor = "\0";
  header->devmajor = "\0";*/

  strcpy(header->magic,"ustar");
  strcpy(header->version, "00");

  prefix_name_split(path, direntp->d_name, name, prefix);
  header_set_name(header, name);
  header_set_prefix(header, prefix);
  header_set_mode(header, sb->st_mode);
  header_set_uid(header, sb->st_uid);
  header_set_gid(header, sb->st_gid);
  header_set_size(header, sb->st_size, sb->st_mode);
  header_set_mtime(header, sb->st_mtime);
  header_set_chksum(header);
  header_set_typeflag(header, sb->st_mode);
  header_set_linkname(header, sb->st_mode, name); /*TODO is this the same name as above?*/
  header_set_uname(header, sb->st_uid);
  header_set_gname(header, sb->st_gid);

  /**prefix_name_split(path);
  header_set_name(header, );*/

  return header;
}

void traverse_to_root(struct stat* sb, struct stat* sb_list, char *path){
   int i;
   long prev_i;
   int first;

   first = 1;
   prev_i= 0; /*default value should never be used.*/
   i = 0;
   sb_list[i] = *sb;
   i++;
   while((sb->st_ino != prev_i) || first)
   {
      chdir("../");
      prev_i = sb->st_ino;
      stat("./", sb);
      sb_list[i] = *sb;
      i++;

      first = 0;
      /*This is a waste of an assignment every time except the first time.*/
   }
   create_path(path, sb_list, i-OFF_SET);
   return;
}
