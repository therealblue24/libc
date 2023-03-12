CC = clang
CFLAGS = -std=c2x -O2 -g
LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/aarch64/include
TEST_LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/aarch64/include $(BIN)/$(LIB_BIN) -I/opt/homebrew/include -L/opt/homebrew/lib -lcmocka

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
ASMSRC = $(wildcard arch/aarch64/src/*.s)
ASMOBJ = $(ASMSRC:.s=.o)
OBJ = $(SRC:.c=.o)
BIN = bin
TEST_SRC = test/*.c
TEST_BIN = libctests
LIB_BIN = tr24libc.a
AS = clang

.PHONY: all

all: dirs make tests

dirs:
	@mkdir -p ./$(BIN)
	@echo "	MKDIR $(BIN)"

tests: $(OBJ)
	@$(CC) -o $(BIN)/$(TEST_BIN) $(TEST_SRC) $(TEST_LDFLAGS) $^
	@echo "	CC $(TEST_SRC)"

make: $(OBJ) $(ASMOBJ)
	@ar rcs bin/$(LIB_BIN) $(OBJ)
	@echo "	AR $(OBJ)"

%.o: %.c
	@echo "	CC $<"
	@$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

%.o: %.s
	@echo "	AS $<"
	@$(AS) -c -o $@ $<

clean:
	@rm -rf $(OBJ) $(BIN) $(ASMOBJ)
	@echo "	RM $(OBJ) $(BIN) $(ASMOBJ)"
