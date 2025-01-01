CFLAGS := -Wall -Wextra -Wpedantic -Wformat=2 \
	-Wno-unused-parameter -Wshadow -Wwrite-strings \
	-Wstrict-prototypes -Wold-style-definition \
	-Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
	-Wjump-misses-init -Wlogical-op
CFLAGS += -lgmp
CC := gcc

SRC := $(wildcard src/*.c)
TEST_SRC := $(wildcard test/*.c)

OBJ := $(SRC:src/%.c=build/%.o) 
TEST_OBJ := $(TEST_SRC:test/%.c=build/test/%.o) 
TEST_OBJ += $(filter-out src/main.c, $(SRC))

bin/out: $(OBJ)
	@ $(CC) $(CFLAGS) $^ -lgmp -o $@ 

build/%.o : src/%.c
	@ $(CC) -c $(CFLAGS) $< -o $@

build/test/%.o : test/%.c 
	@ $(CC) -c $(CFLAGS) $< -o $@

run:
	@ ./bin/out

.PHONY: test
test: $(TEST_OBJ)
	@ $(CC) $(CFLAGS) $^ -lgmp -o bin/test
	@ ./bin/test

.PHONY: test-mem
test-mem: $(TEST_OBJ)
	@ $(CC) $(CFLAGS) $^ -lgmp -o bin/test
	@ valgrind --leak-check=full --track-origins=yes ./bin/test

.PHONY: clear
clear:
	rm -r build
	mkdir build
