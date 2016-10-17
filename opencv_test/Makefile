# Configuration area
CXX = g++
LDFLAGS = -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video
CXXFLAGS = -O0 $(LDFLAGS)
PROJ_DIR := opencv_test
OUTPUT := test.out


objects := $(patsubst %.cpp,%.o,$(wildcard $(PROJ_DIR)/*.cpp))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o $@

clean:
	rm $(PROJ_DIR)/*.o

