CFLAGS=-Wall -Werror -g

.PHONY: clean run-main run-interactive

run-main: main
	./main

run-interactive: interactive
	./interactive

interactive: interactive.c
	gcc interactive.c imageprocessing.c $(CFLAGS) -o interactive

main: main.c
	gcc main.c imageprocessing.c $(CFLAGS) -o main

clean:
	rm -f main interactive
