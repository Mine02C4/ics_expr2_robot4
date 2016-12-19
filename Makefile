MODULES := motor_control i2c_sensor gun_turret

CXX := g++
CXXFLAGS := -O2 -std=c++11 -Wall
STUBLDFLAGS = `pkg-config --libs opencv`
CORE_DIR := core
BUILD_DIR := build
OUTPUT := core.out
TESTELF := test.out

OBJS := $(patsubst $(CORE_DIR)%,$(BUILD_DIR)%,$(patsubst %.cpp,%.o,$(wildcard $(CORE_DIR)/*.cpp)))
DEPS := $(patsubst %.o,%.d, $(OBJS))
MOD_OBJS := $(addsuffix /mod.o,$(MODULES))
MOD_STUBS := $(addsuffix /stub.o,$(MODULES))

.PHONY: all test clean subclean fclean $(MOD_OBJS) $(MOD_STUBS)

all: $(OUTPUT)

test: $(TESTELF)

$(OUTPUT): $(OBJS) $(MOD_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TESTELF): $(OBJS) $(MOD_STUBS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -Lstub_library -lstub $(STUBLDFLAGS)

$(MOD_OBJS):
	$(MAKE) -C $(@D) mod

$(MOD_STUBS):
	$(MAKE) -C $(@D) stub

$(BUILD_DIR)/%.o: $(CORE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -MMD -MT '$@' -o $@ $<

-include $(DEPS)

clean:
	rm -f $(BUILD_DIR)/*

fclean: clean
	$(foreach c,$(MODULES),$(MAKE) -C $(c) clean && )true
	$(MAKE) -C stub_library clean

