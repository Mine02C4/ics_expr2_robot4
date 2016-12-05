CXX = g++
LDFLAGS = `pkg-config --libs opencv`
CXXFLAGS = -O2 -std=c++11
BUILD_DIR := build
OUTPUT := libstub.a

OBJS := $(addprefix $(BUILD_DIR)/,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))
DEPS := $(patsubst %.o,%.d, $(OBJS))

.PHONY: mod clean

mod: $(OUTPUT)

$(OUTPUT): $(OBJS)
	ar ruv $@ $(OBJS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -MMD -MT '$@' -o $@ $<

-include $(DEPS)

clean:
	rm -f $(BUILD_DIR)/* $(OUTPUT)

