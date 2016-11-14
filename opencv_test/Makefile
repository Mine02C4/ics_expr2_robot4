# Configuration area
CXX = g++
LDFLAGS = -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video
CXXFLAGS = -O2 -std=c++11
>>>>>>> 6f7c000bb6c934a9840805b56dab2027151a4662
PROJ_DIR := opencv_test
BUILD_DIR := build
OUTPUT := test.out

OBJS := $(patsubst $(PROJ_DIR)%,$(BUILD_DIR)%,$(patsubst %.cpp,%.o,$(wildcard $(PROJ_DIR)/*.cpp)))
DEPS := $(patsubst %.o,%.d, $(OBJS))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: $(PROJ_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -MMD -MT '$@' -o $@ $<

-include $(DEPS)

clean:
	rm $(BUILD_DIR)/*

