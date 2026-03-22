# esh — A Unix Shell

A minimal Unix shell implemented in C, built as a personal project
to explore system programming concepts.

## Features

- Execute external programs via `fork` / `execvp`
- I/O Redirection (`>`, `<`, `>>`)
- Pipe (`|`)
- Built-in commands:
  - `cd` — change directory
  - `pwd` — print working directory
  - `echo` — print arguments
  - `export` — set environment variables
  - `unset` — unset environment variables
  - `exit` — exit the shell

## Build

​```bash
make
​```

## Usage

​```bash
./esh
​```

## Project Structure

​```
esh/
├── src/        # Source files
├── include/    # Header files with Doxygen comments
├── test/       # Test cases
└── Makefile
​```
