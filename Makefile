srcdir = src

CFLAGS += -Wall -Wextra

mysh: $(srcdir)/main.c
	gcc $(srcdir)/main.c $(CFLAGS) -o $@

clean:
	rm mysh