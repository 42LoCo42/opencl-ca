CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lOpenCL -lreadline

.PHONY: run clean

run: main
	./$<

clean:
	rm -f main

submodules:
	git submodule init
	git submodule update
	touch $@

main: main.c clutils.c mazegen/mazegen.c

mazegen/mazegen.c: submodules
