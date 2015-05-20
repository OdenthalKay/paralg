################################################################################
#
# compile and install library of utility functions
#
# commands:
# make		: compiles library
# make install	: installs library
# make test	: compiles test programs
# make run	: run test programs
#
################################################################################
# compiler, flags, etc.

# installation directory
INSTALL_DIR	= /usr/local/libFHBRS

# GNU C compiler
CC		= cc
CFLAGS		= -std=c99 -g -O0 -Wall -Wstrict-prototypes 
CFLAGS_SHARED	= $(DEFINES) $(CFLAGS) -shared

# Intel C compiler
#CC		= icc
#CFLAGS		= -std=c99 -g -O2 -Wall -Wstrict-prototypes
#CFLAGS_SHARED	= $(DEFINES) $(CFLAGS) -shared

# libraries
LDLIBS		= -L. -lFHBRS -L/usr/X11R6/lib -lX11 -lpthread -lm

# object files
OBJECTS		= time.o rand.o randp.o graphic.o


################################################################################
# general rules

.SUFFIXES:
.SUFFIXES: .c .o .a

# C
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

# Linking
%.exe: %.o
	$(CC) -o $@ $^ $(LDLIBS)

################################################################################
# rules

.PHONY: all install run clean

all:: libFHBRS.a

libFHBRS.a: $(OBJECTS)
	$(AR) rcv $@ $(OBJECTS)

install::
	cp libFHBRS.h libFHBRS.a $(INSTALL_DIR)/

test:: timetest.exe graphictest.exe randtest.exe

run:: timetest.exe randtest.exe graphictest.exe
	./timetest.exe
	./randtest.exe
	./graphictest.exe

clean::
	-rm -f *.o *.a *.exe

################################################################################
# single file rules

graphic.o: graphic.c libFHBRS.h

time.o: time.c libFHBRS.h

rand.o: rand.c libFHBRS.h

randp.o: randp.c libFHBRS.h

graphictest.exe: graphic.c libFHBRS.h
	$(CC) $(CFLAGS) -DTEST -o $@ $< $(LDLIBS)

timetest.exe: timetest.o libFHBRS.a
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

################################################################################
