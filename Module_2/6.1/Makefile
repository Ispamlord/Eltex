CC = gcc
CFLAGS = -Wall -std=c11

LIB = libcontact_list.a
OBJS = contact_list.o
EXEC = main

all: $(EXEC)

$(LIB): contact_list.c
	$(CC) $(CFLAGS) -c $< -o $(OBJS)
	ar rcs $@ $(OBJS)

$(EXEC): main.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -lcontact_list -o $@

clean:
	rm -f $(OBJS) $(LIB) $(EXEC)
