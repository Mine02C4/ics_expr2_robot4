CC = gcc
CFLAGS = -Wall

OUTPUT := motor.out
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(patsubst %.c,%.d,$(SRCS))
MAIN := main
STUB := stub

.PHONY: all mod stub clean

all: $(OUTPUT)

mod: mod.o

mod.o: $(filter-out $(MAIN).o $(STUB).o, $(OBJS))
	ar ruv mod.o $^

stub: stub.o

$(OUTPUT): $(filter-out $(STUB).o, $(OBJS))
	$(CC) $(CFLAGS) $^ -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

-include $(DEPS)

clean:
	rm -f *.o *.d $(OUTPUT)

