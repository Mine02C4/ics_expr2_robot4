# Configuration area
CXX = g++
CXXFLAGS = -O0
PROJ_DIR := opencv_test
OUTPUT := opencv_test

objects := $(patsubst %.cpp,%.o,$(wildcard $(PROJ_DIR)/*.cpp))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(objects)
	$(CXX) $(objects) -o $@

clean:
	rm $(PROJ_DIR)/*.o

