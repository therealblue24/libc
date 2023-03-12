CC = clang
CFLAGS = -std=c2x -O2 -g
LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/aarch64/include
TEST_LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/aarch64/include $(BIN)/$(LIB_BIN)

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin
TEST_SRC = test/*.c
TEST_BIN = libctests
LIB_BIN = tr24libc.a

.PHONY: all

all: dirs make tests

dirs:
	@mkdir -p ./$(BIN)
	@echo "	MKDIR $(BIN)"

tests: $(OBJ)
	@$(CC) -o $(BIN)/$(TEST_BIN) $(TEST_SRC) $(TEST_LDFLAGS) $^
	@echo "	CC $(TEST_SRC)"

make: $(OBJ)
	@ar rcs bin/$(LIB_BIN) $(OBJ)
	@echo "	AR $(OBJ)"

%.o: %.c
	@echo "	CC $<"
	@$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "	RM $(OBJ) $(BIN)"
