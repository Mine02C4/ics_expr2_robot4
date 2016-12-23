# Configuration area
CXX = g++
LDFLAGS = `pkg-config --libs opencv`
CXXFLAGS = -O2 -std=c++11
PROJ_DIR := opencv_test
BUILD_DIR := build
OUTPUT := test.out

OBJS := $(patsubst $(PROJ_DIR)%,$(BUILD_DIR)%,$(patsubst %.cpp,%.o,$(wildcard $(PROJ_DIR)/*.cpp)))
DEPS := $(patsubst %.o,%.d, $(OBJS))

.PHONY: all clean test

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(PROJ_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -MMD -MT '$@' -o $@ $<

-include $(DEPS)

clean:
	rm $(BUILD_DIR)/*

test: all
	(cd $(PROJ_DIR) && ../$(OUTPUT))

