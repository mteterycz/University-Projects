CC = gcc
CFLAGS = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu17 -fPIC -O2
LDFLAGS = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

.PHONY: all clean

all: libnand.so testy

nand.o: nand.c nand.h
memory_tests.o: memory_tests.c memory_tests.h
nand_example.o: nand_example.c nand.h memory_tests.h

libnand.so: nand.o memory_tests.o
	$(CC) -shared $(LDFLAGS) -o $@ $^
	
testy: nand_example.o libnand.so
	$(CC) -L. $< -lnand -o $@

clean:
	rm -f testy *.o *.so
