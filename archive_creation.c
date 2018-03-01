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

/*given: the path (aquired by getpwd()), a char array of 100, 
 * and a char array size 155*/
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
    mode[0] = '0';
    mode[1] = '0';
    mode[2] = '0';
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
   int i;
   i = 0;
   for(; i < 8; i++){
      header->chksum[i] = ' ';  
   }
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
  header_set_linkname(header, sb->st_mode, name); 
  /*TODO is this the same name as above?*/
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

static void shove_it_in(char* buffer, char* field, int index, int length){
   int j;
   for(j = 0; j < length; j++){
      buffer[index+j] = field[j];  
   }
}

static int sum_up(unsigned char * buffer, int size){
   int i, count;
   i = 0;
   count = 0;
   while(i < size){
      count += (int)buffer[i];
      i += 1;
   }
   return count;
}

void write_header(Header *header, int fd){
   char buffer[BLOCK_SIZE];
   unsigned char *bptr;
   char chksum[8];
   int i, j, k,total_sum;
   i = 0;
   j = 0;
   k = 0;
   clear_char_array(buffer, BLOCK_SIZE);
   
   shove_it_in(buffer,header->name, i, NAME_SIZE);
   i += NAME_SIZE;
   shove_it_in(buffer,header->mode, i, MODE_LEN);
   i += MODE_LEN;
   shove_it_in(buffer,header->uid, i, UID_LEN);
   i += UID_LEN;
   shove_it_in(buffer, header->gid, i, GID_LEN);
   i += GID_LEN;
   shove_it_in(buffer, header->size, i, SIZE_LEN);
   i += SIZE_LEN;
   shove_it_in(buffer, header->mtime, i, MTIME_LEN);
   i += MTIME_LEN;
   j = i;
   shove_it_in(buffer, header->chksum, i, CHKSUM_LEN);
   i += CHKSUM_LEN;
   shove_it_in(buffer, &header->typeflag, i, TYPEFLAG_LEN);
   i += TYPEFLAG_LEN;
   shove_it_in(buffer, header->linkname, i, LINKNAME_LEN);
   i += LINKNAME_LEN;
   shove_it_in(buffer, header->magic, i, MAGIC_LEN);
   i += MAGIC_LEN;
   shove_it_in(buffer, header->version, i, VERSION_LEN);
   i += VERSION_LEN;
   shove_it_in(buffer, header->uname, i, UNAME_LEN);
   i += UNAME_LEN;
   shove_it_in(buffer, header->gname, i, GNAME_LEN);
   i += GNAME_LEN;
   shove_it_in(buffer, header->devmajor, i, DEVMAJOR_LEN);
   i += DEVMAJOR_LEN;
   shove_it_in(buffer, header->devminor, i, DEVMINOR_LEN);
   i += DEVMINOR_LEN;
   shove_it_in(buffer, header->prefix, i, PREFIX_LEN);
   i += DEVMINOR_LEN;
  
   bptr = buffer;
   total_sum = sum_up(bptr, BLOCK_SIZE);
   while(k <= 6){
      chksum[6-k] = (char) ((total_sum % 8) + ASCII_NUM_OFFSET);
      total_sum /= 8;
      k++;
   }
   chksum[7] = '\0';
   strcpy(header->chksum, chksum);
   shove_it_in(buffer, header->chksum, j, CHKSUM_LEN);
   write(fd, buffer, BLOCK_SIZE);
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



/*Given a path and a tarfile archive the 
 * entry at the given path into the tarfile. */
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
