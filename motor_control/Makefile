CC = gcc
CFLAGS = -Wall

OUTPUT := motor.out
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(patsubst %.c,%.d,$(SRCS))
MAIN := main

.PHONY: all mod stub clean

all: $(OUTPUT)

mod: mod.o

mod.o: $(filter-out $(MAIN).o, $(OBJS))
	ar ruv mod.o $^

stub: stub.o

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

-include $(DEPS)

clean:
	rm -f *.o *.d $(OUTPUT)

