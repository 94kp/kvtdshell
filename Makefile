CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = shell ops mkdir rmdir

all: $(TARGETS)

shell: shell.c
	$(CC) $(CFLAGS) shell.c -o shell

ops: ops.c
	$(CC) $(CFLAGS) ops.c -o ops


mkdir: mkdir.c
	$(CC) $(CFLAGS) mkdir.c -o mkdir

rmdir: rmdir.c
	$(CC) $(CFLAGS) rmdir.c -o rmdir

run: all
	./shell

clean:
	rm -f $(TARGETS) *.txt

.PHONY: all run clean
