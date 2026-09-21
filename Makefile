srcdir = src
bindir = bin

CC = gcc
CFLAGS += -Wall -Wextra

# FILE = bin/mysh

mysh: $(srcdir)/main.c
	mkdir $(bindir)/
	gcc $(srcdir)/main.c $(CFLAGS) -o $(bindir)/$@

clean:
# # 	ifeq ("$(wildcard $(FILE))","")
# 	ifeq ($(CC),gcc)
# 		echo "No executable exists."
# 	else
# 		rm $(bindir)/mysh
# 	endif
	rm $(bindir)/mysh
