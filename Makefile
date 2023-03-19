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
	@echo "made dir $(BIN)"

tests: $(OBJ)
	@$(CC) -o $(BIN)/$(TEST_BIN) $(TEST_SRC) $(TEST_LDFLAGS) $^
	@echo "compiled test $(TEST_SRC)"

make: $(OBJ) $(ASMOBJ)
	@ar rcs bin/$(LIB_BIN) $(OBJ)
	@echo "made lib $(LIB_BIN)"

%.o: %.c
	@echo "compiled $<"
	@$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

%.o: %.s
	@echo "assembled $<"
	@$(AS) -c -o $@ $<

clean:
	@rm -rf $(OBJ) $(BIN) $(ASMOBJ)
	@echo "cleaned"
