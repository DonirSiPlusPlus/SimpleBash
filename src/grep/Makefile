CC=gcc
CFLAGS=-Wall -Werror -Wextra #-std=c11 -pedantic # -fsanitize=address
OS=$(shell uname)
ifeq ($(OS), Linux)
 LIBS=-lpthread -lrt -lm -lsubunit
 OPEN=xdg-open
 LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./test.out
else
 OPEN=open
 LEAKS=CK_FORK=no leaks --atExit -- ./test.out
endif

all: s21_grep clang check leaks

clean:
	rm -Rf *.o s21_grep *.txt vgcheck *.dSYM

s21_grep: s21_grep.o grep.o
	$(CC) $(CFLAGS) s21_grep.o grep.o -o s21_grep

test: access
	./test_grep.sh

access:
	chmod a+rwx test_grep.sh

val: 
	$(CC) -O0 -g s21_grep.c grep.c -o vgcheck
	valgrind --leak-check=full ./test_grep.sh

clang:
	clang-format -i *.c *.h
	clang-format -n *.c *.h

leaks: access
	leaks -atExit -- ./test_grep.sh

check:
	cppcheck *.c *.h

s21_grep.o: s21_grep.c
	$(CC) -c s21_grep.c

grep.o: grep.c
	$(CC) -c grep.c