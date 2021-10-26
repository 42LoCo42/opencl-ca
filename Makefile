CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lOpenCL -lreadline

run: main
	./$<

main: main.c clutils.c

clean:
	rm -f main

.PHONY: run clean
