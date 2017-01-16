JULIUSDIR := ../julius-4.2.3
LIBSENT := $(JULIUSDIR)/libsent
LIBJULIUS := $(JULIUSDIR)/libjulius

CXX := g++
OUTPUT := voice_recog.out
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
MOD_OBJS := $(patsubst %.cpp,%.mod.o,$(SRCS))
MAIN := main
CXXFLAGS=-O2 -g -std=c++11 -I$(LIBJULIUS)/include -I$(LIBSENT)/include  `$(LIBSENT)/libsent-config --cflags` `$(LIBJULIUS)/libjulius-config --cflags`
LDFLAGS= -L$(LIBJULIUS) `$(LIBJULIUS)/libjulius-config --libs` -L$(LIBSENT) `$(LIBSENT)/libsent-config --libs`

############################################################

.PHONY: all mod stub

all: $(OUTPUT)

mod: mod.o

mod.o: $(filter-out $(MAIN).mod.o, $(MOD_OBJS))
	ar rcs mod.o $^

$(OUTPUT): $(filter-out $(STUB).o, $(OBJS))
	$(CXX) -o $@ $^ $(LDFLAGS)

%.mod.o: %.cpp
	$(CXX) $(CXXFLAGS) -DMOD_MODE $(LDFLAGS) -c -MMD -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -MMD -o $@ $<

clean:
	$(RM) *.o *.bak *~ core TAGS

distclean:
	$(RM) *.o *.bak *~ core TAGS
	$(RM) julius-simple
