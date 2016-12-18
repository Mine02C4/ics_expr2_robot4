CC := gcc
CFLAGS := -Wall
LDFLAGS := -l

OUTPUT := turret.out
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(patsubst %.c,%.d,$(SRCS))
MAIN := main
STUB := stub
STUBLIB := ../stub_library/libstub.a

.PHONY: all mod stub clean $(STUBLIB)

all: $(OUTPUT)

mod: mod.o

mod.o: $(filter-out $(MAIN).o $(STUB).o, $(OBJS))
	ar ruv mod.o $^

stub: stub.o $(STUBLIB)

$(STUBLIB):
	$(MAKE) -C $(@D)

$(OUTPUT): $(filter-out $(STUB).o, $(OBJS))
	$(CC) $(CFLAGS) $^ -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

-include $(DEPS)

clean:
	rm -f *.o *.d $(OUTPUT)

