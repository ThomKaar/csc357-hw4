/*Written for God by His grace. */

#include "archive_creation.h"

/*This file's purpose is to store helper functions to
 Archive files for mytar. */


static int insert_special_int(char*where, size_t size, int32_t val){
   /* For interoperability with GNU tar.  GNU seems to
      * * set the highorder bit of the first byte, then
      * * treat the rest of the field as a binary integer
      * * in network byte order.
      * * Insert the given integer into the given field
      * * using this technique.  Returns 0 on success, nonzero
      * * otherwise
      * **/
   int err=0;
   if( val<0||( size< sizeof(val)) ){
      /* if its negative, bit 31 is set and we cant use the flag
      * * if len is too small, we cant write it.  Either way, were
      * * done.
      * *
      */
      err++;
   }
   else{
      /* game on....**/
      memset(where, 0, size);
      /*  Clear out the buffer **/
      *(int32_t *)(where+size-sizeof(val)) = htonl(val);
      /* place the int **/
      *where|= 0x80;
      /* set that highorder bit **/
   }
   return err;
}

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
  clear_char_array(name, 100);
  clear_char_array(prefix, 155);
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
    if(st_mode > MAX_OCTAL_IN_DECIMAL){
       insert_special_int(header->mode, 8, st_mode);
       return;      
    }
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
  
  header->uidflag = 0;
  if(uid > MAX_OCTAL_IN_DECIMAL){
      insert_special_int(header->uid, 8, uid);
      header->uidflag = 1;
      return;
  }
  
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

  header->gidflag = 0;
  if(gid > MAX_OCTAL_IN_DECIMAL){
      insert_special_int(header->gid, 8, gid);
      header->gidflag = 1;
      return;
  }
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
    while(i <= 10){
      header_size[10-i] = (char) ((size % 8) + ASCII_NUM_OFFSET);
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
  tim[11] = '\0';
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

static int header_compute_chksum(Header *header, char* field, int size){
   int i;
   int count;
   i = 0; /* initialized i here --A */
   count = 0;
   while(i < size)
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
   int i;
   i = 0;
   char chksum[8];
   clear_char_array(chksum, 8);
   c = 0;
   c += header_compute_chksum(header, header->devminor, 8);
   c += header_compute_chksum(header, header->devmajor, 8);
   c += USTAR_ASCII_SUM;
   c += VERSION_ASCII_SUM;
   c += header_compute_chksum(header, header->gname, 32);
   c += header_compute_chksum(header, header->uname, 32);
   c += header_compute_chksum(header, header->linkname, 100);
   c += header->typeflag;
   c += header_compute_chksum(header, header->mtime, 12);
   c += header_compute_chksum(header, header->size, 12);
   c += header_compute_chksum(header, header->name, 100);
   c += header_compute_chksum(header, header->mode, 8);
   c += header_compute_chksum(header, header->uid, 8);
   c += header_compute_chksum(header, header->gid, 8);
   c += header_compute_chksum(header, header->prefix, 155); 
   while(i <= 6){
      chksum[6-i] = (char) ((c % 8) + ASCII_NUM_OFFSET);
      c /= 8;
      i++;
    }
    chksum[7] = '\0';
    strcpy(header->chksum, chksum);
  
   chksum[7] = '\0';
   strncpy(header->chksum, chksum, 8);

   return;
}

Header * create_header(char * path){
  char name[100];
  char prefix[155];
  struct stat *sb;
  Header *header = (Header*) malloc(sizeof(Header));
  sb = (struct stat *) malloc(sizeof(struct stat));

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

   write(fd,header->name, sizeof(header->name));
   write(fd, header->mode, sizeof(header->mode));
   write(fd, header->uid, sizeof(header->uid));
   write(fd, header->gid, sizeof(header->gid));
   write(fd, header->size, sizeof(header->size));
   write(fd, header->mtime, sizeof(header->mtime));
   write(fd, header->chksum, sizeof(header->chksum));
   write(fd, &header->typeflag, sizeof(char));
   write(fd, header->linkname, sizeof(header->linkname));
   write(fd, header->magic, sizeof(header->magic));
   write(fd, header->version, sizeof(header->version));
   write(fd, header->uname, sizeof(header->uname));
   write(fd, header->gname, sizeof(header->gname));
   write(fd, header->devmajor, sizeof(header->devmajor));
   write(fd, header->devminor, sizeof(header->devminor));
   write(fd, header->prefix, sizeof(header->prefix));
}


/*Given a file path to write blocks of BLOCK_SIZE bytes of data*/
void write_file(int rfd, int wfd){
   char buffer[BLOCK_SIZE];
   int valid_read;
   clear_char_array(buffer, BLOCK_SIZE);

   while((valid_read = read(rfd, buffer, sizeof(buffer))) >  INVALID_READ){
         write(wfd, buffer, sizeof(buffer));
         clear_char_array(buffer, BLOCK_SIZE);
   }

   clear_char_array(buffer, BLOCK_SIZE);
   write(wfd, buffer, sizeof(buffer));
   write(wfd, buffer, sizeof(buffer));
}



/*Given a path and a tarfile archive the entry at the given path into the tarfile. */
void write_entry(char * path, char *tarfile, int rfd, int wfd){
   Header *header;
   struct stat* sb;
   sb = (struct stat *) malloc(sizeof(struct stat));
   lstat(path, sb);
   header = (Header*) malloc(sizeof(Header));
   header = create_header(path);
   write_header(header, wfd); 
   if(S_ISREG(sb->st_mode)){
      write_file(rfd, wfd);   
   }
   free(header);
}
