CC = gcc
CFLAGS = -fPIC -Wall -Wextra -std=c11
LDFLAGS = -shared
BIN = calculator
LIB_DIR = libs
SRC = calculator.c
LIB_SRC = sum.c subtraction.c multiply.c division.c

LIBS = $(LIB_SRC:.c=.so)

all: $(BIN)

$(BIN): $(SRC) $(LIBS)
	$(CC) -o $@ $(SRC) -ldl

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

%.so: %.c | $(LIB_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIB_DIR)/$@ $<

clean:
	rm -f $(BIN)
	rm -rf $(LIB_DIR)

.PHONY: all clean
