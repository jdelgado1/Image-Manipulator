#Jose Delgado
#jdelgad9
#Sean O'Connor
#soconn19

# Some variable definitions to save typing later on
CC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

# Links the hw3 executable
project: project.o imageManip.o ppm_io.o
	$(CC) -o project project.o imageManip.o ppm_io.o -lm

# Compiles project.c into an object file
project.o: project.c imageManip.h ppm_io.h
	$(CC) $(CFLAGS) -c project.c

# Compiles imageManip.c into an object file
imageManip.o: imageManip.c imageManip.h ppm_io.h
	$(CC) $(CFLAGS) -c imageManip.c -lm

# Compiles imageManip.c into an object file
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# 'make clean' will remove intermediate & executable files
clean:
	rm -f *.o project

