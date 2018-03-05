/* Written for God by his infinite grace. */

#include "unpack_helper.h"

/*given a file descriptor in the spot at the top of the header. */
void store_name(char * buf, D_Node * node){
   char buffer0[256];
   char buffer1[100];
 
   strncpy(buffer0,buf+345, 155);
   strncpy(buffer1, buf, 100);
   strcat(buffer0, buffer1);
   strcat(buffer0, buffer1);
   strcpy(node->name,  buffer0);
   return;
}

/*given a file descriptor at spot 100 of a header, store the mode as an long*/
void store_mode(char* buf, D_Node *node){
   char buffer[8];
   long mode;
   char *ptr;
   
   strncpy(buffer, buf+100, 8);
   mode = strtol(buffer, &ptr, 10);
   node->sb->st_mode = mode;
   return;
}

/*given a file descriptor at spot 108 of a header, store the uid as a long. */
void store_uid(char *buf, D_Node *node){
   char buffer[8];
   char * ptr;
   long uid;
   strncpy(buffer, buf+108, 8);
   uid = strtol(buffer,&ptr, 10);
   node->sb->st_uid = uid;
   return;
}


void store_gid(char * buf, D_Node *node){
   char buffer[8];
   char * ptr;
   long gid;

   strncpy(buffer, buf+116, 8);
   gid = strtol(buffer,&ptr, 10);
   node->sb->st_gid = gid;
   return;
}

void store_size(char * buf, D_Node *node){
   char buffer[12];
   char* ptr;
   long size;

   strncpy(buffer, buf+124, 12);
   size = strtol(buffer,&ptr, 10);
   node->sb->st_size = size;
}

void store_mtime(char * buf, D_Node * node){
   char buffer[12];
   char *ptr;
   long mtime;
   strncpy(buffer, buf+136, 12);
   mtime = strtol(buffer,&ptr, 10);
   node->sb->st_mtime = mtime;
}

/*given a file descriptor at the chksum spot, move to typeflag and store it */
void store_typeflag(char * buf, D_Node *node){
   char buffer[1]; 
   strncpy(buffer, buf+156, 1);
   node->filetype = *buffer;  
   return;
}

/*given a file descriptor at the linknamespot, move to uname and store it.*/
void store_uname(char *buf, D_Node *node){
   char buffer[32];
   
   strncpy(buffer, buf+265, 32);
   strcpy(node->uname, buffer);
}

void store_gname(char * buf, D_Node *node){
   char buffer[32];
   strncpy(buffer, buf+297, 32);
   strcpy(node->gname, buffer);
}


void store_one(char *buf, D_Node *node){
   store_name(buf, node);
   store_mode(buf, node);
   store_uid(buf, node);
   store_gid(buf, node);
   store_size(buf, node);
   store_mtime(buf, node);
   store_typeflag(buf, node);
   return;
}


void store_all(char *tarfile){
   char header_buffer[512];
   char block_buffer[512];
   int rfd;
   int block_count;
   int bytes_read;

   D_Node *node;
   node = (D_Node *) malloc(sizeof(D_Node));
   rfd = open(tarfile, O_RDONLY);
   while((bytes_read = read(rfd, header_buffer, sizeof(header_buffer)) > 0)){
      store_one(header_buffer, node);
      if((block_count =  node->sb->st_size / BLOCK_SIZE) > 0){
         while(block_count > 0){
            read(rfd, block_buffer, sizeof(block_buffer));
            /*do what we need with the block buffer */

         }
      }
      else{
         /*File is either a directory or a symlink*/
         if(node->filetype == '2'){
            /*It is a symbolic link */
         }
         else if(node->filetype == '5'){
            /*It is a directory */
         }
      }
   }
}
   


