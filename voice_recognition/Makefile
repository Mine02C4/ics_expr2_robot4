LIBSENT=../libsent
LIBJULIUS=../libjulius

<<<<<<< HEAD
CC=g++
CFLAGS=-g -O2 
=======
CC=gcc
CFLAGS=-g -O2 -Wall
MAIN := main
>>>>>>> 6f7c000bb6c934a9840805b56dab2027151a4662

####
#### When using system-installed libraries
####
# CPPFLAGS=`libjulius-config --cflags` `libsent-config --cflags`
# LDFLAGS=`libjulius-config --libs` `libsent-config --libs`

####
#### When using within-package libraries
####
CPPFLAGS=-I$(LIBJULIUS)/include -I$(LIBSENT)/include  `$(LIBSENT)/libsent-config --cflags` `$(LIBJULIUS)/libjulius-config --cflags`
LDFLAGS= -L$(LIBJULIUS) `$(LIBJULIUS)/libjulius-config --libs` -L$(LIBSENT) `$(LIBSENT)/libsent-config --libs`

############################################################

.PHONY: all mod stub

all: julius-simple

mod: mod.o

mod.o: $(filter-out $(MAIN).o, $(OBJS))
	ar ruv mod.o $^

julius-simple: julius-simple.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o julius-simple julius-simple.c $(LDFLAGS)

clean:
	$(RM) *.o *.bak *~ core TAGS

distclean:
	$(RM) *.o *.bak *~ core TAGS
	$(RM) julius-simple
