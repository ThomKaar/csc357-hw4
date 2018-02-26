/*Written for God by His grace. */

#include "archive_creation.h"

/*This file's purpose is to store helper functions to
 archive files for mytar. */

void traverse_to_root(struct stat* sb, struct stat *sb_list, char *path)
{
   int i
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

static void header_set_name(Header* header, char* name){
      strcpy(header->name, name);
}

/*given: the path (aquired by getpwd()), a char array of 100, and a char array size 155*/
static void prefix_name_split(char*path, char*d_name, char*name, char* prefix){
  int i;
  int offset;

  if(strlen(d_name) > 100){
    offset = strlen(d_name) - 100;
    strcpy(name, (d_name + offset);

    strcpy(prefix, path);
    strncat(prefix, d_name, offset);
  }
  else{
    strcpyn(name, d_name, strlen(d_name));
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
    mode[8] = '\0';
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
  u[8] = '\0';
  strcpy(header->uid, u);
}

static void header_set_gid(Header *header, gid_t gid){
  char g[8];
  int i;

  clear_char_array(g, 8);

  i = 0;
  while(i < 7){
    g[7-i] = (char) ((g % 8) + ASCII_NUM_OFFSET);
    gid /= 8;
    i++;
  }
  g[8] = '\0';
  strcpy(header->gid, g);
}

/**/
static void header_set_size(Header *header, off_t size, int valid_file){
  char header_size[12];
  int i;
  for(i=0;i<12;i++){
    header_size[i] = '0';
  }
  header_size[11] = '\0';
  i = 0;
  if(!valid_file){
    strcpy(header->size, header_size);
  }
  else{
    while(i < 11){
      header_size[11-i] = (char) ((size % 8) + ASCII_NUM_OFFSET);
      size /= 8;
      i++;
    }
    header_size[12] = '\0';
    strcpy(header->gid, size);
  }
}

static void header_set_mtime(Header *header, time_t timespec){
  char time[12];
  clear_char_array(time, 12);
  while(i < 11){
    time[11-i] = (char) ((timespec->st_mtime % 8) + ASCII_NUM_OFFSET);
    timespec /= 8;
    i++;
  }
  time[11] = '\0';
  strcpy(header->st_mtime,time);
  return ;
}

static void clear_char_array(char *array, int size){
  int i;
  for(i=0; i< size; i++){
    array[i] = '\0';
  }
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
  char * namebuffer[100];
  clear_char_array(namebuffer, 100);
  if(S_ISLINK(mode)){
    strcpy(header->linkname, linkname);
  }
  else{
    strcpy(header->linkname, namebuffer);
  }
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

Header * create_header(char * path){
  char name[100];
  char prefix[155];
  struct stat *sb;
  Header * header;
  int cksum;

  cksum = 0;
  lstat(path, sb);
  header->devminor = "\0";
  cksum += 1;
  header->devmajor = "\0";
  cksum += 1;
  header->magic = "ustar";
  cksum += 5;
  header->version = "00";
  cksum += 2;
  header_set_mode(header, sb->st_mode);
  cksum += 8;
  /**prefix_name_split(path);
  header_set_name(header, );*/

  return header;
}
