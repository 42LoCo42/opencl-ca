# CFLAGS := -g -Wall -Wextra
CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lOpenCL

run: main
	./$<

main: main.c clutils.c kernel.cl
	$(CC) $(CFLAGS) $(LDFLAGS) $< clutils.c -o $@

clean:
	rm -f main

.PHONY: run clean
