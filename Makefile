# Change target here to your platform building for!!!!
TARGET = aarch64

CC = clang
CFLAGS = -std=gnu2x -O2 -g
LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/$(TARGET)/include
TEST_LDFLAGS = -ffreestanding -nostdinc -Iinclude/ -Iarch/$(TARGET)/include $(BIN)/$(LIB_BIN) -I/opt/homebrew/include -L/opt/homebrew/lib -lcmocka

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
ASMSRC = $(wildcard arch/$(TARGET)/src/*.s)
ASMOBJ = $(ASMSRC:.s=.o)
OBJ = $(SRC:.c=.o)
BIN = bin
TEST_SRC = test/*.c
TEST_BIN = libctests
LIB_BIN = tr24libc.a
AS = clang

.PHONY: all

all: dirs make

ci_build: $(OBJ) $(ASMOBJ)
	@echo "dont use this in actual applications"
	TARGET=x86_64
	@ar rcs bin/$(LIB_BIN) $(OBJ)
	@echo "made lib $(LIB_BIN)"
	@rm -rf $(OBJ) $(ASMOBJ)

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
