CXX := g++
CXXFLAGS = -g -O2
OUTPUT := text_to_speech.out
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
MAIN := main.cpp
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
	$(RM) text_to_speech
