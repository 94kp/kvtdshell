CC = gcc
CFLAGS = -Wall -Wextra -g

TARGETS = shell ops mkdir rmdir touch cat ls

all: $(TARGETS)

shell: shell.c
	$(CC) $(CFLAGS) shell.c -o shell

ops: ops.c
	$(CC) $(CFLAGS) ops.c -o ops

mkdir: mkdir.c
	$(CC) $(CFLAGS) mkdir.c -o mkdir

rmdir: rmdir.c
	$(CC) $(CFLAGS) rmdir.c -o rmdir

touch: touch.c
	$(CC) $(CFLAGS) touch.c -o touch

cat: cat.c
	$(CC) $(CFLAGS) cat.c -o cat

ls: ls.c
	$(CC) $(CFLAGS) ls.c -o ls
run: all
	./shell

clean:
	rm -f $(TARGETS) *.txt
	rm -rf *.dSYM

.PHONY: all run clean
