CC = gcc
CFLAGS = -Wall

OUTPUT := motor.out
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(patsubst %.c,%.d,$(SRCS))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

-include $(DEPS)

clean:
	rm *.o *.d $(OUTPUT)

