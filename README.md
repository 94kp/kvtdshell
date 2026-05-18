# Project Name: kvtdshell
Project 3 for Systems Programming Course

---

## Contributions:
1. <b>Karan Patil</b>: Implemented shell loop, '>' redirection and mkdir & rmdir commands.
2. <b>Devjyot Singh Sidhu</b>: Implemented '<' redirction and touch & cat command.
3. <b>Tanish Sarang Mahajan</b>: Implemented '>>' redirection and cd command.
4. <b>Vedant Pandhare</b>: Implemented the ls command with support for detailed listings, hidden files, and verbose output.

---

## How to run:
Just run make command in shell (prerequisite: gcc compiler) and then run ./shell.

---

## Shell command usage:
### rmdir

### mkdir

### touch
Creates a file if it does not exist, or updates its timestamp if it does.

**Usage:** `touch [-v] [-c] file...`

**Options:**
- `-v` — verbose mode, prints a message describing the action taken
- `-c` — do not create the file if it does not exist, silently skip instead

**Examples:**
- `touch file.txt` — creates file.txt, or updates its timestamp if it already exists
- `touch -v file.txt` — same as above, but prints what action was taken
- `touch -c file.txt` — updates timestamp only if file.txt already exists
- `touch file1.txt file2.txt file3.txt` — touch multiple files at once

### cat
Reads and prints the contents of a file, or reads from standard input if no file is given. Multiple files can be passed and will be printed in order.

**Usage:** `cat [-v] [-n] [file...]`

**Options:**
- `-v` — verbose mode, prints the name of each file before reading it
- `-n` — numbers each output line

**Examples:**
- `cat file.txt` — prints the contents of file.txt
- `cat -n file.txt` — prints the contents of file.txt with line numbers
- `cat file1.txt file2.txt` — prints the contents of both files in order
- `cat < file.txt` — reads file.txt via stdin redirection

### ls
Lists the contents of a directory. By default it shows the current directory, but you can pass any path you want. It also supports multiple directories at once. Hidden files (dot-entries) are skipped unless you explicitly ask for them with -a.

**Usage:** `ls [-l] [-a] [-v] [path ...]`

**Options:**
- `-l` — detailed view, shows permissions, owner, group, file size, and last modified time for each entry
- `-a` — include hidden files and folders (those starting with a dot)
- `-v` — verbose mode, prints which directory is being listed before showing its contents

**Examples:**
- `ls` — lists everything in the current directory
- `ls -l` — same as above but with full file details on each line
- `ls -a` — lists everything including hidden dot-files
- `ls -la` — combines both, detailed view of all files including hidden ones
- `ls -v /some/path` — lists a specific directory and announces which one it's reading
- `ls dir1 dir2` — lists two directories one after the other
- `ls > out.txt` — writes the directory listing into a file using output redirection

---

## Shell redirection Usage:
### >
Redirects the standard output of a command to a file, creating it if it does not exist or overwriting it if it does.

**Usage:** `command > file`

**Examples:**
- `ls > out.txt` — writes the output of ls into out.txt
- `ls > out.txt` — overwrites out.txt if it already exists

### <
Redirects the contents of a file to the standard input of a command.

**Usage:** `command < file`

**Examples:**
- `cat < file.txt` — reads and prints the contents of file.txt via stdin
