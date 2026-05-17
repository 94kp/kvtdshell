CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = shell ls mkdir rmdir

all: $(TARGETS)

shell: shell.c
	$(CC) $(CFLAGS) shell.c -o shell

ls: ls.c
	$(CC) $(CFLAGS) ls.c -o ls


mkdir: mkdir.c
	$(CC) $(CFLAGS) mkdir.c -o mkdir

rmdir: rmdir.c
	$(CC) $(CFLAGS) rmdir.c -o rmdir

run: all
	./shell

clean:
	rm -f $(TARGETS)

.PHONY: all run clean
