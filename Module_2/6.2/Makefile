CC = gcc
CFLAGS = -Wall -std=c11 -fPIC
LDFLAGS = -shared

LIB = libcontact_list.so
EXEC = main

all: $(LIB) $(EXEC)

$(LIB): contact_list.c contact_list.h
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(EXEC): main.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -lcontact_list -o $@
	LD_LIBRARY_PATH=. ./main

clean:
	rm -f $(LIB) $(EXEC)