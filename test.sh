#!/bin/bash

FLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wno-unused-parameter -Wshadow -Wwrite-strings -Wstrict-prototypes -Wold-style-definition -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wjump-misses-init -Wlogical-op -g"

# Compile
gcc -c src/matrix.c -o build/matrix.o 
gcc -c test/main.c -o build/main.o 

# Linking
gcc ${FLAGS} build/matrix.o build/main.o -lgmp -o bin/test

# Running
valgrind -s --leak-check=full --track-origins=yes ./bin/test
