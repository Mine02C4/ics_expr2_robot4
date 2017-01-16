JULIUSDIR := ../julius-4.2.3
LIBSENT := $(JULIUSDIR)/libsent
LIBJULIUS := $(JULIUSDIR)/libjulius

CXX := g++
CXXFLAGS = -g -O2 -std=c++11
OUTPUT := voice_recog.out
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
MAIN := main
CPPFLAGS=-I$(LIBJULIUS)/include -I$(LIBSENT)/include  `$(LIBSENT)/libsent-config --cflags` `$(LIBJULIUS)/libjulius-config --cflags` -std=c++11
LDFLAGS= -L$(LIBJULIUS) `$(LIBJULIUS)/libjulius-config --libs` -L$(LIBSENT) `$(LIBSENT)/libsent-config --libs`

############################################################

.PHONY: all mod stub

all: $(OUTPUT)

mod: mod.o

mod.o: $(filter-out $(MAIN).o, $(OBJS))
	ar rcs mod.o $^

$(OUTPUT): $(filter-out $(STUB).o, $(OBJS))
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -c -MMD -o $@ $<
clean:
	$(RM) *.o *.bak *~ core TAGS

distclean:
	$(RM) *.o *.bak *~ core TAGS
	$(RM) julius-simple
