CC = gcc
CFLAGS = -std=c99 -g -Wall -pedantic
LD = gcc

LDFLAGS = -g 
all : mytar

mytar: main_tar.o archive_creation.o flags_helper.o
	$(LD) $(LDFLAGS) -o mytar main_tar.o archive_creation.o flags_helper.o

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
