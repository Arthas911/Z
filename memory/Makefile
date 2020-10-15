#this a simple makefile
#CC=/opt/ti/linux-devkit/sysroots/x86_64-arago-linux/usr/bin/arm-linux-gnueabihf-gcc
OBJECTS=mm_test
CC=gcc
$(OBJECTS):$(subst .c,.o,$(wildcard *.c))
	$(CC) -o $@ $^ -lm -lpthread
%.o:%.c
	$(CC) -c $< -g -O2 -Wall
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(OBJECTS)
