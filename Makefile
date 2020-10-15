#this a simple makefile
#CC=/opt/xilinx/ctxa9/bin/arm-linux-gnueabihf-gcc
OBJECTS=uart_test

CC=gcc
$(OBJECTS):$(subst .c,.o,$(wildcard *.c))
	$(CC) -o $@ $^ -lm -lpthread
%.o:%.c
	$(CC) -c $< -g -O2 -Wall
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(OBJECTS)
