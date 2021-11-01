CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lOpenCL -lreadline

run: main
	./$<

main: main.c clutils.c mazegen/mazegen.c

clean:
	rm -f main

.PHONY: run clean
