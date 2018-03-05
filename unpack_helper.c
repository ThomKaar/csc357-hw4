/* Written for God by his infinite grace. */

#include "unpack_helper.h"

/*given a file descriptor in the spot at the top of the header. */
void store_name(int rfd, D_Node * node){
   char buffer0[256];
   char buffer1[100];
 
   lseek(rfd,SEEK_CUR, 345);
   read(rfd, buffer0, sizeof(char) * 155);
   lseek(rfd, SEEK_CUR, -500);
   read(rfd, buffer1, sizeof(buffer1));
   strcat(buffer0, buffer1);
   strcpy(node->name,  buffer0);
   return;
}

/*given a file descriptor at spot 100 of a header, store the mode as an long*/
void store_mode(int rfd, D_Node *node){
   char buffer[8];
   long mode;
   read(rfd, buffer, sizeof(buffer));
   mode = octal_to_decimal(buffer, 8);
   node->sb->st_mode = mode;
   return;
}

/*given a file descriptor at spot 108 of a header, store the uid as a long. */
void store_uid(int rfd, D_Node *node){
   char buffer[8];
   long uid;
   read(rfd, buffer, sizeof(buffer));
   uid = octal_to_decimal(buffer, 8);
   node->sb->st_uid = uid;
   return;
}


void store_gid(int rfd, D_Node *node){
   char buffer[8];
   long gid;
   read(rfd, buffer, sizeof(buffer));
   gid = octal_to_decimal(buffer, 8);
   node->sb->st_gid = gid;
   return;
}

void store_size(int rfd, D_Node *node){
   char buffer[12];
   long size;
   read(rfd, buffer, sizeof(buffer));
   size = octal_to_decimal(buffer, 12);
   node->sb->st_size = size;
}

void store_mtime(int rfd, D_Node * node){
   char buffer[12];
   long mtime;
   read(rfd, buffer, sizeof(buffer));
   mtime = octal_to_decimal(buffer, 12);
   node->sb->st_mtime = mtime;
}

/*given a file descriptor at the chksum spot, move to typeflag and store it */
void store_typeflag(int rfd, D_Node *node){
   char buffer[1];
   
   lseek(rfd, SEEK_CUR, 8); 
   read(rfd, buffer, sizeof(buffer));
   node->filetype = buffer;  
   return;
}

/*given a file descriptor at the linknamespot, move to uname and store it.*/
void store_uname(int rfd, D_Node *node){
   char buffer[32];
   lseek(rfd, SEEK_CUR, 99);
   read(rfd, buffer, sizeof(buffer));
   strcpy(node->uname, buffer);
}

void store_gname(int rfd, D_Node *node){
   char buffer[32];
   read(rfd, buffer, sizeof(buffer));
   strcpy(node->gname, buffer);
}


long octal_to_decimal(char* octal_char, int size){
      int n;
      long total, octal;
      n = 0;
      while(n < size){
         octal  = (octal_char[size-(n+1)] -ASCII_NUM_OFFSET);
         total += (pow(8,n) * octal);
      }
      return total;
}

