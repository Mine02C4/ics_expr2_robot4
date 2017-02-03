MODULES := motor_control i2c_sensor gun_turret voice_recognition text_to_speech

CXX := g++
CXXFLAGS := -O2 -std=c++11 -Wall -g -D_GLIBCXX_DEBUG
JULIUSDIR := julius-4.2.3
LIBSENT := $(JULIUSDIR)/libsent
LIBJULIUS := $(JULIUSDIR)/libjulius
JULI_LDFLAGS= -L$(LIBJULIUS) `$(LIBJULIUS)/libjulius-config --libs` -L$(LIBSENT) `$(LIBSENT)/libsent-config --libs`
LDFLAGS := `pkg-config --libs opencv` $(JULI_LDFLAGS)
STUBLDFLAGS =
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
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TESTELF): $(OBJS) $(MOD_STUBS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -Lstub_library -lstub $(STUBLDFLAGS) $(LDFLAGS)

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

