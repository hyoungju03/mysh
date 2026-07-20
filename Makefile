srcdir = src
bindir = bin

CFLAGS += -Wall -Wextra

mysh: $(srcdir)/main.c
	gcc $(srcdir)/main.c $(CFLAGS) -o $(bindir)/$@

clean:
	rm $(bindir)/mysh
