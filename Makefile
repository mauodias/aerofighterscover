#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++
CFLAGS  = -g -std=c++11
LIBRARIES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lCGF-Linux -lz
LIBRARYDIR = -Llib -L./lib/SFML-2.2/lib-Linux64 -Llib/CGF
INCLUDEDIR = -Ilib/CGF/include -Ilib/CGF/include/tmxloader -Ilib/CGF/include/pugixml -Ilib/SFML-2.2/include
TARGET = PlayState
LOCAL = $(shell pwd)

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: PlayState

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
PlayState:  PlayState.o main.o
	$(CC) $(CFLAGS) $(LIBRARYDIR) -o bin/$(TARGET) obj/PlayState.o obj/main.o $(LIBRARIES)
	echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:$(LOCAL)/lib:$(LOCAL)/lib/Box2D-2.3.0/lib-Linux64:$(LOCAL)/lib/SFML-2.2/lib-Linux64 $(LOCAL)/bin/$(TARGET)" > bin/$(TARGET).sh
	chmod +x bin/$(TARGET).sh

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
PlayState.o:
	$(CC) $(CFLAGS) $(INCLUDEDIR) -c PlayState.cpp -o obj/PlayState.o

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
main.o:
	$(CC) $(CFLAGS) $(INCLUDEDIR) -c main.cpp -o obj/main.o

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) bin/$(TARGET) obj/*.o bin/*~ bin/*.sh
