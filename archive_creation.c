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
static void prefix_name_split(char*path, char*name, char* prefix){
  int offset;

  if(strlen(path) > 100){
    offset = strlen(path) - 100;
    strcpy(name, (path + offset));

    strncpy(prefix, path, offset);
  }
  else{
    strcpy(name, path);
  }
}

/*Add a string representation of mode_t as the mode member of header
bitwise or the st_mode with the mode #defines */
static void header_set_mode(Header *header, mode_t st_mode){
    char mode[8];
    int i;

    clear_char_array(mode, 8);
    i = 0;
    while(i <= 6){
      mode[6-i] = (char) ((st_mode % 8) + ASCII_NUM_OFFSET);
      st_mode /= 8;
      i++;
    }
    mode[7] = '\0';
    strncpy(header->mode, mode, 8);
}


/*Given a uid in octal create a string representing the octal values*/
static void header_set_uid(Header *header, uid_t uid){
  char u[8];
  int i;

  clear_char_array(u,8);

  i = 0;
  while(i <= 6){
    u[6-i] = (char) ((uid % 8) + ASCII_NUM_OFFSET);
    uid /= 8;
    i++;
  }
  u[7] = '\0';
  strncpy(header->uid, u,8);
}

static void header_set_gid(Header *header, gid_t gid){
  char g[8];
  int i;

  clear_char_array(g, 8);

  i = 0;
  while(i <= 6){
    g[6-i] = (char) ((gid % 8) + ASCII_NUM_OFFSET);
    gid /= 8;
    i++;
  }
  g[7] = '\0'; /* Changed from 8 to 7 to be within range */
  strncpy(header->gid, g,8);
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
    return;
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
  while(i <= 10){
    tim[10-i] = (char) ((timespec % 8) + ASCII_NUM_OFFSET);
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
  else if(S_ISLNK(mode)){
    header->typeflag = '2';
  }
  else if(S_ISDIR(mode)){
    header->typeflag = '5';
  }
}

static void header_set_linkname(Header * header, mode_t mode, char* linkname){
  char namebuffer[100];
  clear_char_array(namebuffer, 100);
  if(S_ISLNK(mode)){
    strcpy(header->linkname, linkname);
  }
  else{
    strcpy(header->linkname, namebuffer);
  }
}

static int header_compute_chksum(Header *header, char* field){
   int i;
   int count;
   i = 0; /* initialized i here --A */
   while(field[i] != '\0')
   {
      count += field[i];
      i++;
   }
   return count;
}

static void header_set_uname(Header* header, uid_t uid){
  struct passwd *pw;
  pw = getpwuid(uid);
  pw->pw_name[31] = '\0';
  strncpy(header->uname, pw->pw_name, 32);
}

static void header_set_gname(Header* header, gid_t gid){
  struct group *gr;
  gr = getgrgid(gid);
  gr->gr_name[31] = '\0';
  strncpy(header->gname, gr->gr_name, 32);
}

static void header_set_chksum(Header *header)
{
   int c;
   c = 0;
   c += header_compute_chksum(header, header->devminor);
   c +=header_compute_chksum(header, header->devmajor);
   c += USTAR_ASCII_SUM;
   c += VERSION_ASCII_SUM;
   c += header_compute_chksum(header, header->gname);
   c += header_compute_chksum(header, header->uname);
   c += header_compute_chksum(header, header->linkname);
   c +=header->typeflag;
   c += header_compute_chksum(header, header->mtime);
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
  Header *header = (Header*) malloc(sizeof(Header));
  sb = (struct stat *) malloc(sizeof(struct stat));

  header->chksum = 0;

  lstat(path, sb);
 
  clear_char_array(header->devminor, 8);
  clear_char_array(header->devmajor, 8);


  strcpy(header->magic,"ustar");
  strcpy(header->version, "00");

  prefix_name_split(path, name, prefix);
  header_set_name(header, name);
  header_set_prefix(header, prefix);
  header_set_mode(header, sb->st_mode);
  header_set_uid(header, sb->st_uid);
  header_set_gid(header, sb->st_gid);
  header_set_size(header, sb->st_size, sb->st_mode);
  header_set_mtime(header, sb->st_mtime);
  header_set_typeflag(header, sb->st_mode);
  header_set_linkname(header, sb->st_mode, name); /*TODO is this the same name as above?*/
  header_set_uname(header, sb->st_uid);
  header_set_gname(header, sb->st_gid);
  header_set_chksum(header);
 
  
  return header;
}



void traverse_down_one(char * path){
        
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
   /*create_path(path, sb_list, i-OFF_SET);*/
   return;
}

void write_header(Header *header, int fd){
   
   write(fd,header->name, sizeof(uint8_t)*100);
   write(fd, header->mode, sizeof(uint8_t)*8);
   write(fd, header->uid, sizeof(uint8_t)*8);
   write(fd, header->gid, sizeof(uint8_t)*8);
   write(fd, header->size, sizeof(uint8_t)*12);
   write(fd, header->mtime, sizeof(uint8_t)*10);
   write(fd, &header->chksum, sizeof(uint8_t));
   write(fd, &header->typeflag, sizeof(uint8_t));
   write(fd, header->linkname, sizeof(uint8_t)*100);
   write(fd, &header->magic, sizeof(uint8_t)*6);
   write(fd, &header->version, sizeof(uint8_t)*3);
   write(fd, header->uname, sizeof(uint8_t)*32);
   write(fd, header->gname, sizeof(uint8_t)*32);
   write(fd, &header->devmajor, sizeof(uint8_t)*8);
   write(fd, &header->devminor, sizeof(uint8_t)*8);
   write(fd, header->prefix, sizeof(uint8_t)*155);
}


