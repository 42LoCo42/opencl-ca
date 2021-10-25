# CFLAGS := -g -Wall -Wextra
CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lOpenCL

run: main
	./$<

main: main.c clutils.c

clean:
	rm -f main

.PHONY: run clean
