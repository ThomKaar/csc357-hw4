CC = gcc
CFLAGS = -std=c99 -g -Wall -pedantic
LD = gcc

LDFLAGS = -g 
all : mytar

mytar: main_tar.o archive_creation.o flags_helper.o unpack_helper.o directory_struct.o
	$(LD) $(LDFLAGS) -o mytar main_tar.o archive_creation.o flags_helper.o unpack_helper.o directory_struct.o

unpack_helper.o : unpack_helper.c
	$(LD) $(LDFLAGS) -c -o unpack_helper.o unpack_helper.c

directory_struct.o : directory_struct.c
	$(LD) $(LDFLAGS) -c -o directory_struct.o directory_struct.c

archive_creation.o : archive_creation.c
	$(LD) $(LDFLAGS) -c -o archive_creation.o archive_creation.c

flags_helper.o : flags_helper.c
	$(LD) $(LDFLAGS) -c -o flags_helper.o flags_helper.c

main_tar.o : main_tar.c
	$(LD) $(LDFLAGS) -c -o main_tar.o main_tar.c

clean :
	rm -f *.o mytar

spleen :
	rm -f *.o
