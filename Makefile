srcdir = src

mysh: $(srcdir)/main.c
	gcc $(srcdir)/main.c -o $@

clean:
	rm mysh